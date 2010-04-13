<?php

/**
 * Menu callback for the dashboard page.
 */
function nice_dash_dashboard_page() {
	$output = '';

	$widgets = module_invoke_all('nice_dash_widgets');

	if($widgets){

		// Order them in the query
		$result = db_query("SELECT * FROM {nice_dash_widget_values} ORDER BY weight");

		while($row = db_fetch_array($result)){
			$regions[$row['region']] .= call_user_func($widgets[$row['widget_key']]['callback']);
		}
	}

	return theme('nice_dash_page', $regions);

}

/**
 * Menu callback for the dashboard page.
 */
function nice_dash_dashboard_settings_form() {

	$form = array();

	$widgets = module_invoke_all('nice_dash_widgets');

  $form = array();
  $form['#tree'] = TRUE;
  $form['#theme'] = 'nice_dash_settings_form';

	if($widgets){
		foreach($widgets as $key => $value){
			//Fetch values
			$default_values = nice_dash_widget_values($key);

			$form['#widgets'][] = $key;

			$form[$key]['title'] = array('#value' => $value['title']);
			$form[$key]['region'] = array(
				'#type' => 'select',
				'#options' => nice_dash_regions(),
				'#default_value' => $default_values['region'],
				'#attributes' => array(
					'class' => 'field-region-select field-region-disabled ',
				),
			);
			$form[$key]['weight'] = array('#type' => 'weight', '#delta' => 1, '#default_value' => $default_values['weight']);
		}
	}

	$form['widgets'] = array('#type' => 'value', '#value' => $form['#widgets']);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

	return $form;

}

/**
 * Settings form submit handler
 */
function nice_dash_dashboard_settings_form_submit(&$form, &$form_state) {
	$values = $form_state['values'];
	if (sizeof($values['widgets']) > 0) {
		foreach($values['widgets'] as $key){
			$widget = new stdClass();
			$update_keys = array();

			// Check if row exists
			$wid = db_result(db_query("SELECT wid FROM {nice_dash_widget_values} WHERE widget_key = '%s'", $key));
			if(is_numeric($wid)){
				$widget->wid = $wid;
				$update_keys = array('wid');
			}

			$widget->widget_key = $key;
			$widget->region = $values[$key]['region'];
			$widget->weight = $values[$key]['weight'];

			drupal_write_record('nice_dash_widget_values', $widget, $update_keys);
		}
	}
}
