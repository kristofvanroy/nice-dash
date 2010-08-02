<?php

/**
 * @file
 * Scheduler plugin for Nice dashboard.
 */

class nice_dash_scheduler {

  /**
   * Enabled method.
   */
  function is_enabled() {
    return (module_exists('scheduler'));
  }

  /**
   * Views api method.
   */
  function views_api() {
    return TRUE;
  }

  /**
   * Widgets method.
   */
  function widgets() {
    return array(
      'scheduler_unpublish' => t('Content to be unpublished'),
      'scheduler_publish' => t('Content to be published'),
    );
  }

  /**
   * Content to be unpublished
   */
  function nice_dash_scheduler_widget_scheduler_unpublish() {
    return array(
      'title' => t('Scheduled to unpublish'),
      'content' => views_embed_view('nice_dash_scheduler', 'block_4'),
    );
  }

  /**
   * Content to be unpublished
   */
  function nice_dash_scheduler_widget_scheduler_publish() {
    return array(
      'title' => t('Scheduled to publish'),
      'content' => views_embed_view('nice_dash_scheduler', 'block_5'),
    );
  }
}
