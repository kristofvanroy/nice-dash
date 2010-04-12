<?php

/**
 * Theme callback for the dashboard components
 */
function theme_nice_dash_component($title, $description, $content){
  $output .= '<div class="admin-panel">';
  $output .= '<h3><strong>'.$title.'</strong></h3>';
  $output .= '<p class="description">'.$description.'</p>';
  $output .= $content;
  $output .= '</div>';
  return $output;
}

/**
 * Theme callback for the dashboard page
 */
function theme_nice_dash_page($regions){
  drupal_add_css(drupal_get_path('module', 'nice_dash') .'/css/nice_dash.css');
	$output = '<div id="nice-dashboard">';
	$output .= '<div>'. $regions['header'] .'</div>';
	$output .= '<div class="left">'. $regions['left'] .'</div>';
	$output .= '<div class="right">'. $regions['right'] .'</div>';
	$output .= '<div class="clear">'. $regions['footer'] .'</div>';
	$output .= '</div>';
  return $output;
}


/**
 * Preprocess function from the dashboard setting form
 *
 * Here the regions are added
 */
function template_preprocess_nice_dash_settings_form(&$vars) {
  $form = &$vars['form'];

  // Order the widgets
  $order = array();
  foreach ($form['#widgets'] as $key => $field) {
    $order[$field] = $form[$field]['weight']['#default_value'];
  }
  asort($order);

	$rows = array();
  foreach ($order as $key => $field) {
    $element = &$form[$key];
    $row = new stdClass();

    // Each field will have a region, store it temporarily
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
  drupal_add_js(drupal_get_path('module', 'nice_dash') .'/js/nice_dash.js');
  drupal_add_css(drupal_get_path('module', 'nice_dash') .'/css/nice_dash.css');

	$regions = nice_dash_regions();

	foreach ($regions as $region => $title) {
    drupal_add_tabledrag('fields', 'match', 'sibling', 'field-region-select', 'field-region-'. $region, NULL, FALSE);
    drupal_add_tabledrag('fields', 'order', 'sibling', 'field-weight', 'field-weight-'. $region);
  }

  $vars['rows'] = $rows;
  $vars['submit'] = drupal_render($form);
	$vars['regions'] = $regions;
}
