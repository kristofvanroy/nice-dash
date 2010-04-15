<?php

/**
 * Menu callback for the dashboard page.
 */
function nice_dash_dashboard_page($did = NULL) {

  $widgets = module_invoke_all('nice_dash_widgets');

  if($widgets){

		// Fetch the default dashboard
		if(!$did){
			$did = db_result(db_query("SELECT did FROM {nice_dash_dashboards} ORDER BY weight ASC LIMIT 1"));
		}

		$result = db_query("SELECT * FROM {nice_dash_widgets} WHERE did = %d ORDER BY weight", $did);

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

function nice_dash_dashboard_overview_page(){
	$result = db_query("SELECT did, name FROM nice_dash_dashboards ORDER BY weight ASC");
	$headers = array(t('Name'), t('Actions'));
	$rows = array();

	while($row = db_fetch_array($result)){
		$rows[] = array(
			l($row['name'], 'admin/dashboard'),
			l(t('Edit'), 'admin/dashboard/configure/'.$row['did'].'/edit').' | '.t('Delete'));
	}

	return theme('table', $headers, $rows);
}


/**
 * Menu callback for the dashboard settings page.
 */
function nice_dash_dashboard_settings_form(&$form_state, $did = NULL) {
	$form = array();

	if(isset($did)){
		$dashboard = nice_dash_dashboard_values($did);
	}
  $widgets = module_invoke_all('nice_dash_widgets');

  if($widgets){

		$form['#tree'] = TRUE;
		$form['#theme'] = 'nice_dash_settings_form';

    foreach($widgets as $key => $value){
      //Fetch values
      $default_values = nice_dash_widget_values($key, $did);

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
		$form = array();
    drupal_set_message(t('You have to enable at least one of the Nice Dashboard plugin modules. Goto the <a href="@modules-page">modules page</a> and enable one or more of the Nice Dashboard plugins.',array('@modules-page' => url("admin/build/modules"))),'warning');
  }

	$form['dashboard_name'] = array(
		'#type' => 'textfield',
		'#default_value' => $dashboard['name'],
		'#title' => t('Dashboard name'),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => TRUE,
		'#weight' => -100,
	);

	// Current dashboard id
	$form['dashboard_id'] = array('#type' => 'value', '#value' => $did);

  return $form;
}

function nice_dash_dashboard_settings_form_validate(&$form, &$form_state){
	// CHECK IF THE NAME OF THE DASHBOARD DOES NOT YET EXIST

}

/**
 * Settings form submit handler
 */
function nice_dash_dashboard_settings_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];

	// Dashboard

	$update_keys = array();
	$dashboard = new stdClass();
	$dashboard->name = $values['dashboard_name'];

	if(isset($values['dashboard_id'])){
		$update_keys = array('did');
	}

	drupal_write_record('nice_dash_dashboards', $dashboard, $update_keys);

	if(!isset($values['dashboard_id'])){
		$values['dashboard_id'] = db_last_insert_id('nice_dash_dasboard','did');
	}

	// Widgets

  if (sizeof($values['widgets']) > 0) {
    foreach($values['widgets'] as $key){
      $widget = new stdClass();
      $update_keys = array();

      // Check if row exists
      $wid = db_result(db_query("SELECT wid FROM {nice_dash_widgets} WHERE widget_key = '%s' AND did = %d", $key, $values['dashboard_id']));
      if(is_numeric($wid)){
        $widget->wid = $wid;
        $update_keys = array('wid');
      }
			$widget->did = $values['dashboard_id'];
      $widget->widget_key = $key;
      $widget->region = $values[$key]['region'];
      $widget->weight = $values[$key]['weight'];

      drupal_write_record('nice_dash_widgets', $widget, $update_keys);
    }
  }

	module_invoke('menu','rebuild');
}