<?php
/**
 * @file
 * Contains a class defining extraction/formatting methods.
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
    $image = $wrapper->value();
    $url = NULL;

    // If an image style is specified, get path for style.
    if (isset($settings['image_style'])) {
      $url = image_style_url($settings['image_style'], $image['uri']);
    }
    // If no image stype is specified, return path to full image.
    else {
      $url = file_create_url($image['uri']);
    }

    return $url;
  }

  /**
   * Processor for fields of type 'list_integer'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processListintegerType($wrapper, $index, $settings) {
    return $this->processNumberintegerType($wrapper, $index, $settings);
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
  public function processNumberintegerType($wrapper, $index, $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for properties of type 'int'.
   *
   * Reference to $this->processNumberintegerType()
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field from which values are being extracted.
   * @param int $index
   *   Integer representing the delta of the field being processed.
   * @param array $settings
   *   Variable for passing settings and context into processors.
   */
  public function processIntegerType($wrapper, $index, $settings) {
    return $this->processNumberintegerType($wrapper, $index, $settings);
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
   *   Allowed Values:
   *     - processor: Instance of processor class that should be
   *       used to process the referenced entity.
   *     - include_fields: Array of fields => settings that should
   *       be extracted and returned fron referenced entity.
   *
   * @return StdClass|Array
   *   Field values as specified by include_fields and as extracted 
   *   by specified processor class.
   */
  public function processEntityReferenceType($wrapper, $index, $settings) {
    // Get entity info, and entity object from wrapper.
    $entity_info = $wrapper->info();
    $entity = $wrapper->value();

    // If a processor class isn't passed in, use default.
    if (!isset($settings['processor'])) {
      $settings['processor'] = new DistillProcessor();
    }

    // Create an entity distiller.
    $distiller = new Distill($entity_info['type'], $entity, $settings['processor']);

    if (isset($settings['include_fields']) && !empty($settings['include_fields'])) {
      foreach ($settings['include_fields'] as $field_name => $field_settings) {
        // If no field settings were specified, use
        // field value instead of index as name.
        if (is_int($field_name)) {
          $field_name = $field_settings;
          $field_settings = array();
        }
        $distiller->setField($field_name, $field_settings);
      }
    }
    else {
      $distiller->setAllFields();
    }

    return $distiller->getFieldValues();
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
