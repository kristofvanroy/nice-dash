<?php

/**
 * @file
 * Content plugin for Nice dashboard.
 */

class nice_dash_content {

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
   * Widgets method.
   */
  function widgets() {
    return array(
      'content_add' => t('Add content'),
      'content_latest' => t('Latest content'),
      'content_sticky' => t('Sticky content'),
      'content_promoted' => t('Promoted content'),
    );
  }

  /**
   * Add content widget.
   */
  function nice_dash_content_widget_content_add() {
    $item = menu_get_item('node/add');
    $content = system_admin_menu_block($item);
    $content = theme('node_add_list', $content);
    return array(
      'title' => t('Create content'),
      'description' => t('Select your content type'),
      'content' => $content,
    );
  }

  /**
   * Latest content widget.
   */
  function nice_dash_content_widget_content_latest() {
    return array(
      'title' => t('Latest content'),
      'description' => t('An overview of the latest content'),
      'content' => views_embed_view('nice_dash_content', 'block_1'),
    );
  }

  /**
   * Sticky content widget.
   */
  function nice_dash_content_widget_content_sticky() {
    return array(
      'title' =>t('Sticky items'),
      'description' => t('An overview of all content set as sticky'),
      'content' => views_embed_view('nice_dash_content', 'block_2'),
    );
  }

  /**
   * Promoted content widget.
   */
  function nice_dash_content_widget_content_promoted() {
    return array(
      'title' => t('Promoted to frontpage'),
      'description' => t('An overview of all content promoted to frontpage'),
      'content' => views_embed_view('nice_dash_content', 'block_3'),
    );
  }
}
