<?php
/**
 * @file
 * Provides a class that allows values to be easily
 * extracted from an entity object.
 */

/**
 * A class that uses the EntityMetadataWrapper
 * to extract values from an entity object.
 */
class EntityDistill {

  /**
   * @property EntityMetatdataWrapper $entityWrapper
   *   Entity metadata wrapper of the entity from which
   *   values will be extracted.
   */
  public $entityWrapper;

  /**
   * @property EntityDistillProcessor $processor
   *   EntityDistillProcessor object that will be used to process raw field data.
   *   Defaults to new instance of EntityDistillProcessor.
   */
  public $processor;

  /**
   * @property string $language
   *   Language code of language that should be used when
   *   extracting field data. Defaults to LANGUAGE_NONE.
   */
  public $language = LANGUAGE_NONE;

  /**
   * @property array $values
   *   Array of values extracted from entity, keyed by
   *   property name.
   */
  public $values = array();

  /**
   * @property array $processableFieldTypes
   *   Array of field types found on this class's entity,
   *   and a boolean indicating whether or not the type is
   *   processable by this class.
   */
  public $processableFieldTypes = array();

  /**
   * @property array $processableFields
   *   Array of fields names found on this class's entity,
   *   and a boolean indicating whether or not the type is
   *   processable by this class.
   */
  public $processableFields = array();

  /**
   * Construct a new EntityDistill object.
   *
   * @param string $entity_type
   *   Type of entity that a wrapper is being loaded for.
   * @param stdClass|EntityDrupalWrapper $entity
   *   Entity that a wrapper is being loaded for, or an entity wrapper.
   * @param EntityDistillProcessor $processor
   *   EntityDistillProcessor object that will be used to process raw field data.
   * @param string $language
   *   Language code of language that should be used when
   *   extracting field data. Defaults to LANGUAGE_NONE.
   */
  public function __construct($entity_type, $entity, $processor = NULL, $language = NULL) {
    // Load entity metadata wrapper.
    if (is_a($entity, 'EntityDrupalWrapper')) {
      $this->entityWrapper = $entity;
    }
    else {
      $this->entityWrapper = entity_metadata_wrapper($entity_type, $entity);
    }

    // Define which fields and field types can/cannot be processed by this class.
    $this->setProcessableFields();
    $this->setProcessableFieldTypes();

    // Load EntityDistillProcessor.
    if ($processor && get_parent_class($processor) == 'EntityDistillProcessor') {
      $this->processor = $processor;
    }
    else {
      $this->processor = new EntityDistillProcessor();
    }

    // If language is passed in, set class language.
    if ($language) {
      $this->language = $language;
    }

  }

  /**
   * Determine which field types can be processed.
   *
   * @see field_info_field_types()
   */
  protected function setProcessableFieldTypes() {
    foreach ($this->entityWrapper->getPropertyInfo() as $field) {
      if ($field['type']) {
        $function_name = 'process' . $this->machineNameToFunctionName($field['type']) . 'Type';
        $acceptable = FALSE;
        if (method_exists($this->processor, $function_name)) {
          $acceptable = TRUE;
        }
        $this->processableFieldTypes[$field['type']] = $acceptable;
      }
    }
    die(var_dump($this->processableFieldTypes));
  }

  /**
   * Determine which fields can be processed.
   */
  protected function setProcessableFields() {
    foreach ($this->entityWrapper->getPropertyInfo() as $field_name => $field) {
      $function_name = 'process' . $this->machineNameToFunctionName($field_name) . 'Field';
      $acceptable = FALSE;
      if (method_exists($this->processor, $function_name)) {
        $acceptable = TRUE;
      }
      $this->processableFields[$field_name] = $acceptable;
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
    // Turn _ into ' ' so that words can be capitalized.
    $function_name = str_replace('_', ' ', $machine_name);
    // Capitalize words.
    $function_name = ucwords($function_name);
    // Remove spaces.
    $function_name = str_replace(' ', '', $function_name);

    return $function_name;
  }
}
