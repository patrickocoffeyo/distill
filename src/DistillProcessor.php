<?php
namespace Drupal\distill;

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
    $this->systemFieldTypes = array_keys(\Drupal::service('plugin.manager.field.field_type')->getDefinitions());
  }
}
