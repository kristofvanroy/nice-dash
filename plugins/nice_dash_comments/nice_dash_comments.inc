<?php

/**
 * @file
 * Comments plugin for Nice dashboard.
 */

class nice_dash_comments {

  /**
   * Enabled method.
   */
  function is_enabled() {
    return (module_exists('comment'));
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
      'latest_comments' => t('Latest comments'),
    );
  }

  /**
   * Latests comments.
   */
  function nice_dash_comments_widget_latest_comments() {
    return theme('nice_dash_component', t('Latest comments'), t('An overview of the latest comments'), views_embed_view('nice_dash_comments', 'block_1'));
  }
}