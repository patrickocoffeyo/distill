<?php
/**
 * @file
 * Contains \Drupal\distill\Distill.
 */

namespace Drupal\distill;

/**
 * Definition of \Drupal\distill\Distill.
 */
class Distill {

  public $entity;
  public $type;
  public $id;
  public $bundle;
  public $language = \Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED;
  public $values = array();
  public $processableFieldTypes = array();
  public $processableFields = array();

  /**
   * Construct a new Distill object.
   *
   * @param $entity
   *   Entity interface that will be processed.
   * @param DistillProcessor $processor
   *   DistillProcessor object that is used to process field data.
   * @param string $language
   *   Language code of language that should be used when
   *   extracting field data. Defaults to \Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED.
   */
  public function __construct($entity, $processor = NULL, $language = NULL) {
    // Set properties.
    $this->entity = $entity;
    $this->type = $entity->getEntityTypeId();
    $this->bundle = $entity->bundle();
    $this->id = $entity->id();
    $this->fieldable = method_exists($this->entity, 'getFieldDefinitions');

    // If this is not a fieldable entity, there's no need for a processor.
    // Just set $this->value to the id.
    if (!$this->fieldable) {
      $this->values = array('target_id' => $this->id);
      return;
    }

    // Load DistillProcessor.
    if ($processor && get_parent_class($processor) === 'DistillProcessor') {
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
    foreach ($this->entity->getFieldDefinitions() as $field_name => $field) {
      $info = $field->getFieldStorageDefinition();
      $type = $info->getType();

      // Check to see if processor can process field of this type.
      $function_base_name = 'process' . $this->machineNameToCamelCase($type) . 'Type';
      $this->processableFieldTypes[$type] = method_exists($this->processor, $function_base_name);

      // Check to see if processor has a function to process this field.
      $function_base_name = 'process' . $this->machineNameToCamelCase($field_name) . 'Field';
      $this->processableFields[$field_name] = method_exists($this->processor, $function_base_name);
    }
  }

  /**
   * Builds a string that corresponds with the name of an extraction function.
   *
   * @param string $machine_name
   *   Underscore-separated machine name of a field or field type.
   *
   * @return string
   *   Camel Case version of the passed in machine name.
   */
  protected function machineNameToCamelCase($machine_name) {
    // Turn into an underscored function.
    $function_name = $this->machineNameToUnderscore($machine_name);
    // Replace _ with  ' ' so that words can be capitalized.
    $function_name = str_replace('_', ' ', $function_name);
    // Capitalize words.
    $function_name = ucwords($function_name);
    // Remove spaces.
    $function_name = str_replace(' ', '', $function_name);

    return $function_name;
  }

  /**
   * Builds a string that corresponds with the name of an extraction hook.
   *
   * @param string $machine_name
   *   Underscore-separated machine name of a field or field type.
   *
   * @return string
   *   Underscore version of the passed in machine name.
   */
  protected function machineNameToUnderscore($machine_name) {
    // Remove >.
    $function_name = str_replace('>', '', $machine_name);
    // Replace < with _
    $function_name = str_replace('<', '', $function_name);

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
   *   Processor configuration and context.
   */
  public function setField($name, $property_name = NULL, $settings = array()) {
    // If this entity isn't fieldable, don't add fields.
    if (!$this->fieldable) {
      return NULL;
    }

    // If field doesn't exist on entity, don't add it.
    if (!$this->entity->hasField($name) || $this->entity->{$name}->isEmpty()) {
      return NULL;
    }

    // Get information about this field that is needed to
    // process it's values later on.
    $field = $this->entity->{$name};
    $info = $field->getFieldDefinition();
    $type = $info->getType();
    $isMultiple = FALSE;

    // If this field is a base field or an entity field, these properties
    // are extracted differently.
    if (get_class($info) === 'Drupal\Core\Field\BaseFieldDefinition') {
      $isMultiple = $info->isMultiple();
    }
    else {
      $isMultiple = $field->count() > 1 ? TRUE : FALSE;
    }

    // Default $property_name to $name.
    if (!$property_name) {
      $property_name = $name;
    }

    // Start an array of field values.
    $field_values = array();

    // Calls proper field processing function.
    // CodeSniffer ignored here because it doesn't
    // understand any sort of lexical scoping.
    // @codingStandardsIgnoreStart
    $process_field = function($type, $field, $index) use ($settings, $name) {
      // If there's a field name function, use it.
      if ($this->processableFields[$name]) {
        $function_name = 'process' . $this->machineNameToCamelCase($name) . 'Field';
      }
      // If there's no field name function, but a type name function, use it.
      elseif ($this->processableFieldTypes[$type]) {
        $function_name = 'process' . $this->machineNameToCamelCase($type) . 'Type';
      }
      // If no field type or name function, implement processor hook function.
      else {
        $function_name = 'distill_process_' . $this->machineNameToUnderscore($type);
        $values = \Drupal::moduleHandler()->invokeAll($function_name, [$field, $index, $settings]);
        if (empty($values)) {
          return NULL;
        }
        else {
          // If $values[0] is empty, but still has value, just return.
          if (!isset($values[0])) {
            return $values;
          }

          return $values[0];
        }
      }

      return $this->processor->{$function_name}($field, $index, $settings);
    };

    // If multivalue field, loop through and extract values.
    if ($isMultiple) {
      // Decide on iterator function name. This changes based on field type.
      $iterator_name = 'getIterator';
      if (get_class($field) === 'Drupal\Core\Field\EntityReferenceFieldItemList') {
        $iterator_name = 'referencedEntities';
      }
      // Loop through and process fields.
      foreach($field->{$iterator_name}() as $index => $field_item) {
        $field_values[] = $process_field($type, $field_item, $index);
      }
    }
    // If single value field, extract single value.
    else {
      $field_values = $process_field($type, $field, 0);
    }

    // Add field value to $this->fieldValues array.
    $this->values[$property_name] = $field_values;
  }
  // @codingStandardsIgnoreEnd

  /**
   * Adds all value of all fields on entity to the $this->values array.
   */
  public function setAllFields() {
    // If this entity is not fieldable, skip.
    if (!$this->fieldable) {
      return;
    }

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
