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


  /**
   * Construct a new EntityDistillProcessor object.
   */
  public function __construct() {
    // Fetch system field types.
    $this->systemFieldTypes = array_keys(field_info_field_types());
  }

  /**
   * Text field processors.
   */
  public function processTextType($wrapper, $index) {
    return $wrapper->value();
  }

  public function processTextLongType($wrapper, $index) {

  }

  public function processTextLongWithSummaryType($wrapper, $index) {

  }

  /**
   * File field processors.
   */
  public function processFileType($wrapper, $index) {

  }

  public function processImageType($wrapper, $index) {

  }

  /**
   * List field processors.
   */
  public function processListIngeterType($wrapper, $index) {

  }

  public function processListFloatType($wrapper, $index) {

  }

  public function processListTextType($wrapper, $index) {

  }

  public function processListBooleanType($wrapper, $index) {

  }

  public function processListTaxonomyTermType($wrapper, $index) {

  }

  public function processListCommentType($wrapper, $index) {

  }

  /**
   * Number and Bool field processors.
   */
  public function processIntegerType($wrapper, $index) {

  }

  public function processNumberIntegerType($wrapper, $index) {

  }

  public function processNumberDecimalType($wrapper, $index) {

  }

  public function processNumberFloatType($wrapper, $index) {

  }

  public function processBooleanType($wrapper, $index) {

  }

  public function processTokenType($wrapper, $index) {

  }

  /**
   * Entity Reference field processors.
   */
  public function processTaxonomyTermReferenceType($wrapper, $index) {

  }

  public function processEntityReferenceType($wrapper, $index) {

  }

  public function processNodeType($wrapper, $index) {

  }

  public function processUserType($wrapper, $index) {

  }

  /**
   * Date field processors.
   */
  public function processDateType($wrapper, $index) {

  }

  /**
   * URI/URL field processors.
   */
  public function processUriType($wrapper, $index) {

  }

}
