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
   * Processor for fields of type 'text'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processTextType($wrapper, $index, $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for fields of type 'text_long'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processTextLongType($wrapper, $index, $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for fields of type 'text_formatted'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processTextWithSummaryType($wrapper, $index, $settings) {
    $field_data = $wrapper->value();

    $field_values = array(
      'value' => check_markup($field_data['value'], $field_data['format']),
    );

    if (isset($field_data['summary']) && !empty($field_data['summary'])) {
      $field_values['summary'] = check_markup($field_data['summary'] . $field_data['format']);
    }

    return $field_values;
  }

  /**
   * Processor for fields of type 'file'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processFileType($wrapper, $index, $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for fields of type 'image'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processImageType($wrapper, $index, $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for fields of type 'list_int'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processListintType($wrapper, $index, $settings) {
    return $this->processNumberintType($wrapper, $index, $settings);
  }

  /**
   * Processor for fields of type 'list_float'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processListFloatType($wrapper, $index, $settings) {
    return $this->processNumberFloatType($wrapper, $index, $settings);
  }

  /**
   * Processor for fields of type 'list_text'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processListTextType($wrapper, $index, $settings) {
    return $this->processTextType($wrapper, $index, $settings);
  }

  /**
   * Processor for fields of type 'list_boolean'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processListBooleanType($wrapper, $index, $settings) {
    return $this->processBooleanType($wrapper, $index, $settings);
  }

  /**
   * Processor for fields of type 'number_int'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processNumberintType($wrapper, $index, $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for properties of type 'int'.
   *
   * Reference to $this->processNumberintType()
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processintType($wrapper, $index, $settings) {
    return $this->processNumberintType($wrapper, $index, $settings);
  }

  /**
   * Processor for fields of type 'number_float'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processNumberFloatType($wrapper, $index, $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for fields of type 'number_decimal'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processNumberDecimalType($wrapper, $index, $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for properties of type 'boolean'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processBooleanType($wrapper, $index, $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for properties of type 'token'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processTokenType($wrapper, $index, $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for fields of type 'entityreference'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processEntityReferenceType($wrapper, $index, $settings) {

  }

  /**
   * Processor for fields of type 'taxonomy_term_reference'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processTaxonomyTermReferenceType($wrapper, $index, $settings) {
    $term = $wrapper->value();
    return $term->tid;
  }

  /**
   * Processor for properties of type 'user'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processUserType($wrapper, $index, $settings) {
    return $this->processEntityReferenceType($wrapper, $index, $settings);
  }

  /**
   * Processor for properties of type 'date'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processDateType($wrapper, $index, $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for properties of type 'uri'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processUriType($wrapper, $index, $settings) {
    return $wrapper->value();
  }
}
