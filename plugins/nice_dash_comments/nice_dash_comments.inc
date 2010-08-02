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
    $widgets = array(
      'comment_configuration' => t('Configure comments'),
    );
    if (module_exists('comment')) {
      $widgets['latest_comments'] = t('Latest comments');
    }
    return $widgets;
  }

  /**
   * Latests comments.
   */
  function nice_dash_comments_widget_latest_comments() {
    return array(
      'title' => t('Latest comments'),
      'description' => t('An overview of the latest comments'),
      'content' => views_embed_view('nice_dash_comments', 'block_1')
    );
  }

  /**
   * Comment configuration.
   */
  function nice_dash_comments_widget_comment_configuration() {

    // Enable or disable button.
    $form = $this->comment_enable_disable_form();

    if (module_exists('comment')) {
      $form += $this->comment_configure_form();
    }
    $form['#submit'][] = 'nice_dash_menu_callback_form_submit';
    $content .= drupal_get_form('nice_dash_menu_callback_form', $form, 'nice_dash_comments', 'comment_configure_form');
    return array(
      'title' => ('Configure comments'),
      'content' => $content,
    );
  }

  /**
   * Enable or disable comments form.
   */
  function comment_enable_disable_form() {
    $form = array();

    $status = module_exists('comment');
    $action = ($status) ? 'Disable' : 'Enable';

    $form['enable'] = array(
      '#type' => 'submit',
      '#value' => t('@action comments', array('@action' => $action)),
    );
    $form['status'] = array(
      '#type' => 'value',
      '#value' => $status,
    );
    return $form;
  }

  /**
   * Comment form configuration.
   */
  function comment_configure_form() {
    $form = array();

    $node_types = node_get_types();
    foreach ($node_types as $type => $node_type) {
      $form['comment_'. $type] = array(
        '#type' => 'select',
        '#title' => $node_type->name,
        '#options' => array(t('Disabled'), t('Read only'), t('Read/Write')),
        '#default_value' => variable_get('comment_'. $type, COMMENT_NODE_READ_WRITE),
      );
    }

    $form = system_settings_form($form);
    $form['buttons']['reset']['#access'] = FALSE;
    return $form;
  }

  /**
   * Submit callback; enable or disable the comments module.
   */
  function comment_configure_form_submit($form, &$form_state) {
    if ($form_state['clicked_button']['#value'] != t('Save configuration')) {
      $status = ($form_state['values']['status']) ? FALSE : TRUE;
      if ($status) {
        module_enable(array('comment'));
      }
      else {
        module_disable(array('comment'));
      }
      drupal_flush_all_caches();
    }
  }

}
