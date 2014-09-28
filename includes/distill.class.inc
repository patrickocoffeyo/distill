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
      $this->entityWrapper = new EntityMetadataWrapper($entity_type, $entity);
    }

    // Define which field types can/cannot be processed by this class.
    $this->setAcceptableFieldTypes();

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
  protected function setAcceptableFieldTypes() {
    $field_types = array_keys(field_info_field_types());
    foreach ($field_types as $type) {
      $function_name = $this->fieldNameToFunctionName($type);
      $acceptable = FALSE;
      if (method_exists($this, $function_name)) {
        $acceptable = TRUE;
      }
      $this->acceptableFieldTypes[$type] = $acceptable;
    }
  }
}
