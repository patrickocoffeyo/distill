<?php
/**
 * @file
 * Contains Distill class.
 */

/**
 * Class that uses EntityMetadataWrapper to extract values from an entity.
 */
class Distill {

  public $entityWrapper;
  public $processor;
  public $language = LANGUAGE_NONE;
  public $values = array();
  public $processableFieldTypes = array();
  public $processableFields = array();

  /**
   * Construct a new Distill object.
   *
   * @param string $entity_type
   *   Type of entity that a wrapper is being loaded for.
   * @param object|EntityDrupalWrapper $entity
   *   Entity that a wrapper is being loaded for, or an entity wrapper.
   * @param DistillProcessor $processor
   *   DistillProcessor object that is used to process field data.
   * @param string $language
   *   Language code of language that should be used when
   *   extracting field data. Defaults to LANGUAGE_NONE.
   */
  public function __construct($entity_type, $entity, DistillProcessor $processor = NULL, $language = NULL) {
    // Load entity metadata wrapper.
    if (is_a($entity, 'EntityDrupalWrapper')) {
      $this->entityWrapper = $entity;
    }
    else {
      $this->entityWrapper = entity_metadata_wrapper($entity_type, $entity);
    }

    // Load DistillProcessor.
    if ($processor && get_parent_class($processor) == 'DistillProcessor') {
      $this->processor = $processor;
    }
    else {
      $this->processor = new DistillProcessor();
    }

    // Define which fields and field types can/cannot be processed.
    $this->setProcessableFieldsAndTypes();

    // If language is passed in, set class language.
    if ($language) {
      $this->language = $language;
    }
  }

  /**
   * Determine which field types can be processed.
   */
  protected function setProcessableFieldsAndTypes() {
    // Loop through fields and check for processability.
    foreach ($this->entityWrapper->getPropertyInfo() as $field_name => $field) {
      // Load field type.
      $field_info = field_info_field($field_name);
      if (!$field_info) {
        $field_info = $field;
      }
      // Get the field type.
      // If the type doesn't exist, default to 'text'.
      if (isset($field_info['type'])) {
        $type = $field_info['type'];
      }
      else {
        $type = 'text';
      }

      // Check to see if processor can process field of this type.
      $function_base_name = 'process' . $this->machineNameToFunctionName($type);
      $this->processableFieldTypes[$type] = method_exists($this->processor, $function_base_name . 'Type');

      // Check to see if processor has a function to process this field.
      $function_base_name = 'process' . $this->machineNameToFunctionName($field_name) . 'Field';
      $this->processableFields[$field_name] = method_exists($this->processor, $function_base_name . 'Field');
    }
  }

  /**
   * Builds a string that corresponds with the name of an extraction function.
   *
   * @param string $machine_name
   *   Underscore-separated machine name of a field or field type.
   *
   * @return string
   *   Name of function corresponding with the passed in machine name.
   */
  protected function machineNameToFunctionName($machine_name) {
    // Remove >.
    $function_name = str_replace('>', '', $machine_name);
    // Replace < with _
    $function_name = str_replace('<', '', $function_name);
    // Replace _ with  ' ' so that words can be capitalized.
    $function_name = str_replace('_', ' ', $function_name);
    // Capitalize words.
    $function_name = ucwords($function_name);
    // Remove spaces.
    $function_name = str_replace(' ', '', $function_name);

    return $function_name;
  }

  /**
   * Adds a field value to the $this->values array.
   *
   * @param string $name
   *   Name of field that should be added to field values array.
   * @param string $property_name
   *   Name of property that will hold the field's value.
   * @param string $settings
   *   Variable for passing settings and context into processors.
   */
  public function setField($name, $property_name = NULL, $settings = array()) {
    // If field doesn't exist on entityWrapper, don't add it.
    if (!$this->entityWrapper->__isset($name)) {
      return NULL;
    }

    // If the field is empty, don't add it.
    $field_wrappers = $this->entityWrapper->{$name};
    $field_wrappers_value = $field_wrappers->value();
    if (empty($field_wrappers_value)) {
      return NULL;
    }

    // Default $property_name to $name.
    if (!$property_name) {
      $property_name = $name;
    }

    // Load field type.
    $field_info = field_info_field($name);
    if (!$field_info) {
      $field_info = $field_wrappers->info();
    }

    // Get the field type.
    $type = $field_info['type'];

    // If there is no function delegated to process this field
    // or this type of field, throw an error.
    if ((!isset($this->processableFieldTypes[$type]) || $this->processableFieldTypes[$type] == FALSE) &&
       (!isset($this->processableFields[$name]) || $this->processableFields[$name] == FALSE)) {
      throw new Exception('EWEntityMetadataWrapperFieldValueExtractor unable to process field "' . $name . '" of type "' . $type . '"');
    }

    // Start an array of field values.
    $field_values = array();

    // Calls proper field processing function.
    // CodeSniffer ignored here because it doesn't
    // understand any sort of lexical scoping.
    // @codingStandardsIgnoreStart
    $process_field = function($type, $wrapper, $index) use ($settings, $name) {
      if ($this->processableFields[$name]) {
        $function_name = 'process' . $this->machineNameToFunctionName($name) . 'Field';
      }
      else {
        $function_name = 'process' . $this->machineNameToFunctionName($type) . 'Type';
      }

      return $this->processor->{$function_name}($wrapper, $index, $settings);
    };

    // If multivalue field, loop through and extract values.
    if (method_exists($field_wrappers, 'count') && $field_wrappers->count() > 0) {
      foreach ($field_wrappers->getIterator() as $index => $wrapper) {
        $field_values[] = $process_field($type, $wrapper, $index);
      }
    }
    // If single value field, extract single value.
    else {
      $field_values = $process_field($type, $field_wrappers, 0);
    }

    // Add field value to $this->fieldValues array.
    $this->values[$property_name] = $field_values;
  }
  // @codingStandardsIgnoreEnd

  /**
   * Adds all value of all fields on entity to the $this->values array.
   */
  public function setAllFields() {
    foreach ($this->entityWrapper->getPropertyInfo() as $field_name => $field) {
      $this->setField($field_name);
    }
  }

  /**
   * Fetches and returns processed field values.
   */
  public function getFieldValues() {
    return $this->values;
  }
}
