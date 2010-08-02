<?php

/**
 * @file
 * Start class for Nice dashboard - has the custom widgets method.
 */

class nice_dash {

  /**
   * Enabled method.
   */
  function is_enabled() {
    return TRUE;
  }

  /**
   * Views api method.
   */
  function views_api() {
    return TRUE;
  }

  /**
   * Return a custom widget.
   */
  function nice_dash_widget_custom($row) {
    $bid = $widget->widget_key;
    $split = explode('-', $bid, 2);
    if (count($split) === 2) {
      list($module, $delta) = $split;
      $block = module_invoke($module, 'block', 'view', $delta);
      if (!empty($block['content'])) {
        if ($module === 'block') {
          global $theme;
          $block['subject'] = db_result(db_query("SELECT title FROM {blocks} WHERE module = 'block' AND delta = '%s'", $delta));
        }
      }
      return array(
        'title' => $widget->title,
        'description' => $widget->description,
        'content' => $block['content'],
      );
    }
  }
}
