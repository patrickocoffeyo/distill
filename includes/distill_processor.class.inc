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
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return string
   *   Value of field.
   */
  public function processTextType($wrapper, $index, array $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for fields of type 'text_long'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return string
   *   Value of field.
   */
  public function processTextLongType($wrapper, $index, array $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for fields of type 'text_formatted'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return array
   *   Value and summary of field.
   */
  public function processTextWithSummaryType($wrapper, $index, array $settings) {
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
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return array
   *   Value of field.
   */
  public function processFileType($wrapper, $index, array $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for fields of type 'image'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *   Allowed Values:
   *     - image_style: string name of image style that should be returned.
   *
   * @return string
   *   Url to image file.
   */
  public function processImageType($wrapper, $index, array $settings) {
    $image = $wrapper->value();
    $url = NULL;

    // If an image style is specified, get path for style.
    if (isset($settings['image_style'])) {
      $url = image_style_url($settings['image_style'], $image['uri']);
    }
    // If no image style is specified, return path to full image.
    else {
      $url = file_create_url($image['uri']);
    }

    return $url;
  }

  /**
   * Processor for fields of type 'list_integer'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return array
   *   Array of field values.
   */
  public function processListintegerType($wrapper, $index, array $settings) {
    return $this->processNumberintegerType($wrapper, $index, $settings);
  }

  /**
   * Processor for fields of type 'list_float'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return array
   *   Array of field values.
   */
  public function processListFloatType($wrapper, $index, array $settings) {
    return $this->processNumberFloatType($wrapper, $index, $settings);
  }

  /**
   * Processor for fields of type 'list_text'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return array
   *   Array of field values.
   */
  public function processListTextType($wrapper, $index, array $settings) {
    return $this->processTextType($wrapper, $index, $settings);
  }

  /**
   * Processor for fields of type 'list_boolean'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return array
   *   Array of field values.
   */
  public function processListBooleanType($wrapper, $index, array $settings) {
    return $this->processBooleanType($wrapper, $index, $settings);
  }

  /**
   * Processor for fields of type 'number_int'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return string
   *   Value of field.
   */
  public function processNumberintegerType($wrapper, $index, array $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for properties of type 'int'.
   *
   * Reference to $this->processNumberintegerType()
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return string
   *   Value of field.
   */
  public function processIntegerType($wrapper, $index, array $settings) {
    return $this->processNumberintegerType($wrapper, $index, $settings);
  }

  /**
   * Processor for fields of type 'number_float'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return string
   *   Value of field.
   */
  public function processNumberFloatType($wrapper, $index, array $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for fields of type 'number_decimal'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return string
   *   Value of field.
   */
  public function processNumberDecimalType($wrapper, $index, array $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for properties of type 'boolean'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return string
   *   Value of field.
   */
  public function processBooleanType($wrapper, $index, array $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for properties of type 'token'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return string
   *   Value of field.
   */
  public function processTokenType($wrapper, $index, array $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for fields of type 'entityreference'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *   Allowed Values:
   *     - processor: Instance of processor class that should be
   *       used to process the referenced entity.
   *     - include_fields: Array of fields => settings that should
   *       be extracted and returned fron referenced entity.
   *
   * @return StdClass|array
   *   Field values as specified by include_fields and as extracted
   *   by specified processor class.
   */
  public function processEntityReferenceType($wrapper, $index, array $settings) {
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
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return string
   *   Term ID of referenced taxonomy term.
   */
  public function processTaxonomyTermReferenceType($wrapper, $index, array $settings) {
    $term = $wrapper->value();
    return $term->tid;
  }

  /**
   * Processor for fields of type 'user'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *   Allowed Values:
   *     - processor: Instance of processor class that should be
   *       used to process the referenced entity.
   *     - include_fields: Array of fields => settings that should
   *       be extracted and returned fron referenced entity.
   *
   * @return StdClass|array
   *   Field values as specified by include_fields and as extracted
   *   by specified processor class.
   */
  public function processUserType($wrapper, $index, array $settings) {
    return $this->processEntityReferenceType($wrapper, $index, $settings);
  }

  /**
   * Processor for properties of type 'date'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return string
   *   Value of field.
   */
  public function processDateType($wrapper, $index, array $settings) {
    return $wrapper->value();
  }

  /**
   * Processor for properties of type 'uri'.
   *
   * @param EntityStructureWrapper|EntityValueWrapper $wrapper
   *   Wrapper of field that is being processed.
   * @param int $index
   *   Delta of field being processed.
   * @param array $settings
   *   Processor configuration and context.
   *
   * @return string
   *   Value of field.
   */
  public function processUriType($wrapper, $index, array $settings) {
    return $wrapper->value();
  }
}
