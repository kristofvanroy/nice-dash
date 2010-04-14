<?php

/**
 * Menu callback for the dashboard page.
 */
function nice_dash_dashboard_page() {

	$widgets = module_invoke_all('nice_dash_widgets');

	if($widgets){

		// Order them in the query
		$result = db_query("SELECT * FROM {nice_dash_widget_values} ORDER BY weight");

		while($row = db_fetch_array($result)){
			if($row['region'] != 'disabled'){
				$regions[$row['region']] .= call_user_func($widgets[$row['widget_key']]['callback']);
			}
		}
	}

	if(!count($regions)){
		drupal_set_message(t('You have not configured any widgets yet. Goto the <a href="@settings-page">dashboard settings</a> and configure some widgets first.',array('@settings-page' => url("admin/dashboard/settings"))),'warning');
		return '';
	}

	return theme('nice_dash_page', $regions);

}

/**
 * Menu callback for the dashboard settings page.
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

		$form['widgets'] = array('#type' => 'value', '#value' => $form['#widgets']);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Save'),
		);
	}
	else {
		drupal_set_message(t('You have to enable at least one of the Nice Dashboard plugin modules. Goto the <a href="@modules-page">modules page</a> and enable one or more of the Nice Dashboard plugins.',array('@modules-page' => url("admin/build/modules"))),'warning');
	}


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
