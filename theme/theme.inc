<?php

/**
 * @file
 * Theming functions for Nice Dashboard.
 */

/**
 * Preprocess function for a dashboard page.
 */
function template_preprocess_nice_dash_page(&$vars) {

  // Set the variable of the region even when empty.
  $regions = nice_dash_regions();
  foreach ($regions as $key => $name) {
    if (!isset($vars['regions'][$key])) {
      $vars['regions'][$key] = '';
    }
  }

  // Add markup.
  drupal_add_js(drupal_get_path('module', 'nice_dash') .'/js/dashboard.js');
  drupal_add_js(drupal_get_path('module', 'ctools') .'/js/collapsible-div.js');
  drupal_add_css(drupal_get_path('module', 'nice_dash') .'/css/nice_dash.css', 'theme');
}

/**
 * Preprocess function for a dashboard component.
 */
function template_preprocess_nice_dash_component(&$vars) {
  $vars['ctools_id'] = $vars['id'] .'_'. strtolower(str_replace(' ', '_', $vars['title']));
}

/**
 * Preprocess function for the dashboard setting form.
 */
function template_preprocess_nice_dash_settings_form(&$vars) {
  $form = &$vars['form'];

  $above_widget_settings = drupal_render($form['dashboard_name']);
  $above_widget_settings .= drupal_render($form['dashboard_access']);
  $above_widget_settings .= drupal_render($form['dashboard_alias']);

  $vars['above_widget_settings'] = $above_widget_settings;
  $vars['below_widget_settings'] = '';

  // Order the widgets
  $order = array();
  if (sizeof($form['#widgets'])) {
    foreach ($form['#widgets'] as $key => $field) {
      $order[$field] = $form[$field]['weight']['#default_value'];
    }
    asort($order);

    $rows = array();
    foreach ($order as $key => $field) {
      $element = &$form[$key];
      $row = new stdClass();

      // Each field will have a region, store it temporarily.
      $region = $element['region']['#default_value'];

      foreach (element_children($element) as $child) {
        if ($child == 'weight') {
          $element['weight']['#attributes']['class'] = 'field-weight field-weight-'. $region;
          $element['weight'] = process_weight($element['weight']);
        }
        $row->{$child} = drupal_render($element[$child]);
      }

      // Add draggable.
      $row->class = 'draggable';
      if ($region == 'disabled') {
        $row->class .= ' region-css-'. $region;
      }
      $rows[$region][] = $row;
    }

    drupal_add_js('misc/tableheader.js');
    drupal_add_js(drupal_get_path('module', 'nice_dash') .'/js/widgets.js');
    drupal_add_css(drupal_get_path('module', 'nice_dash') .'/css/nice_dash.css');

    $regions = nice_dash_regions();

    foreach ($regions as $region => $title) {
      drupal_add_tabledrag('fields', 'match', 'sibling', 'field-region-select', 'field-region-'. $region, NULL, FALSE);
      drupal_add_tabledrag('fields', 'order', 'sibling', 'field-weight', 'field-weight-'. $region);
    }
  }
  $vars['rows'] = $rows;
  $vars['submit'] = drupal_render($form);
  $vars['regions'] = $regions;
}

/**
 * Preprocess function for the dashboard overview form.
 */
function template_preprocess_nice_dash_overview(&$vars) {
  $form = &$vars['form'];
  $rows = array();

  asort($form['#dashboards']);
  foreach ($form['#dashboards'] as $did => $weight) {
    $row = new stdClass;
    $row->title = drupal_render($form[$did]['title']);
    $row->weight = drupal_render($form[$did]['weight']);
    $row->did = drupal_render($form[$did]['did']);
    $rows[] = $row;
  }

  drupal_add_js('misc/tableheader.js');
  drupal_add_tabledrag('fields', 'order', 'sibling', 'overview-weight', NULL, NULL, TRUE);

  $vars['rows'] = $rows;
  $vars['submit'] = drupal_render($form);
}
