<?php
/**
 * @file
 * Provides a class that defines how field values
 * should be extracted, by type and by field.
 */

/**
 * Provides a class that defines how field values should be extracted,
 * by type and by field. Other processing classes can be defined by
 * extending this class and overriding methods.
 */
class EntityDistillProcessor {

  /**
   * @property array systemFieldTypes
   *  Field types available on Drupal system.
   *
   * @see field_info_field_types()
   */
  public $systemFieldTypes;

  public function __construct() {
    // Fetch system field types.
    $this->systemFieldTypes = field_info_field_types();
  }
}
