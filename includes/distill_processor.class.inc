<?php
/**
 * @file
 * Contains a class that defines a processor class.
 *
 * This class can be extended, and processor methods can be added
 * to override hook processors. You can add methods with the following pattern:
 * public function processMyFieldType($wrapper, $index, array $settings).
 * public function processMyFieldName($wrapper, $index, array $settings).
 */

/**
 * Extendable class defining extraction/formatting methods.
 */
class DistillProcessor {

  public $systemFieldTypes;

  /**
   * Construct a new DistillProcessor object.
   */
  public function __construct() {
    // Fetch system field types.
    $this->systemFieldTypes = array_keys(field_info_field_types());
  }
}
