<?php


/**
 * Theme callback for the dashboard page
 */
function template_preprocess_nice_dash_page(&$vars){
	drupal_add_js(drupal_get_path('module', 'nice_dash') .'/js/dashboard.js');
  drupal_add_css(drupal_get_path('module', 'nice_dash') .'/css/nice_dash.css');
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
	if(sizeof($form['#widgets'])){
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
