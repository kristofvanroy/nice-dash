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

		$result = db_query("SELECT * FROM {nice_dash_config} c
											  LEFT JOIN {nice_dash_widgets} w	ON c.wid = w.wid
												WHERE did = %d
												ORDER BY weight", $did);

    while($row = db_fetch_array($result)){
      if($row['region'] != 'disabled'){

				if($row['custom']){
					$widget = nice_dash_get_custom_widget($row);
				}
				else {
				  if (function_exists($widgets[$row['widget_key']]['callback']))  {
  					$widget = call_user_func($widgets[$row['widget_key']]['callback']);
  				}
				}

        $regions[$row['region']] .= $widget;
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
 * Overview page with the dashboards and custom widgets.
 */
function nice_dash_dashboard_overview_page(){

	// Dashboards
	$dashboards = nice_dash_dashboards();
	$output .= '<h2>'.t('Dashboards').'</h2>';

	if(count($dashboards) > 0){
		$headers = array(t('Name'), t('Actions'));
		$rows = array();
		foreach($dashboards as $dashboard){
			$rows[] = array(
				l($dashboard['name'], 'admin/dashboard'),
				l(t('View'), 'admin/dashboard/'.$dashboard['did']).' | '.l(t('Edit'), 'admin/dashboard/configure/'.$dashboard['did'].'/edit').' | '.l(t('Delete'), 'admin/dashboard/configure/'.$dashboard['did'].'/delete'));

		}
		$output .= theme('table', $headers, $rows);
	}
	else {
		drupal_set_message(t('You have to create a dashboard first. You can create your first dashboard on the <a href="@create-page">dashboard create page</a>.',array('@create-page' => url("admin/dashboard/configure/create-dashboard"))), 'warning');
		$output .= '<p>'.t('No dashboards configured.').'</p>';
	}

	$regions['left'] = $output;

	// Widgets
	$output = '';

	$widgets = nice_dash_custom_widgets();
	$output .= '<h2>'.t('Widgets').'</h2>';

	if(count($widgets) > 0){
		$headers = array(t('Title'), t('Actions'));
		$rows = array();
		foreach($widgets as $widget){
			$rows[] = array(
				$widget['title'],
				l(t('Edit'), 'admin/dashboard/widget/'.$widget['wid'].'/edit').' | '.l(t('Delete'), 'admin/dashboard/widget/'.$widget['wid'].'/delete')
				);

		}
		$output .= theme('table', $headers, $rows);
	}
	else {
		$output .= '<p>'.t('No widgets configured.').'</p>';
	}

	$regions['right'] = $output;

  return theme('nice_dash_page', $regions);
}


/**
 * Menu callback for the dashboard settings page.
 */
function nice_dash_dashboard_settings_form(&$form_state, $did = NULL) {
	$form = array();

	if(isset($did)){
		$dashboard = nice_dash_dashboard_values($did);
	}

	$widgets = nice_dash_widgets();

  if($widgets){

		$form['#tree'] = TRUE;
		$form['#theme'] = 'nice_dash_settings_form';

    foreach($widgets as $widget){
      //Fetch values
      $default_values = nice_dash_widget_values($widget['wid'], $did);

      $form['#widgets'][] = $widget['wid'];
      $form[$widget['wid']]['title'] = array('#value' => $widget['title']);
      $form[$widget['wid']]['region'] = array(
        '#type' => 'select',
        '#options' => nice_dash_regions(),
        '#default_value' => $default_values['region'],
        '#attributes' => array(
          'class' => 'field-region-select field-region-disabled ',
        ),
      );
      $form[$widget['wid']]['weight'] = array('#type' => 'weight', '#delta' => 1, '#default_value' => $default_values['weight']);
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
    foreach($values['widgets'] as $wid){

      $widget = new stdClass();
      $update_keys = array();

      // Check if row exists
      $db_wid = db_result(db_query("SELECT wid FROM {nice_dash_config} WHERE wid = %d AND did = %d", $wid, $values['dashboard_id']));

			$widget->wid = $wid;
			$widget->did = $values['dashboard_id'];
      $widget->region = $values[$wid]['region'];
      $widget->weight = $values[$wid]['weight'];

			if(is_numeric($db_wid)){
				$widget->wid = $db_wid;
        $update_keys = array('wid','did');
      }

      drupal_write_record('nice_dash_config', $widget, $update_keys);
    }
  }

	module_invoke('menu','rebuild');

	// Success
	drupal_set_message(t('Saved dashboard.'));

}

/**
 * Confirm delete form for dashboards
 */
function nice_dash_dashboard_delete_form($form_state, $did) {
  $form = array();
  $form['did'] = array('#value' => $did, '#type' => 'value');
	$dashboard = nice_dash_dashboard_values($did);

	if(empty($dashboard)){
		drupal_set_message(t('The dashboard you want to delete does not exist. Go back to the <a href="@overview-page">dashboard overview page</a>.', array('@overview-page' => url("admin/dashboard/configure"))),'warning');
		return $form;
	}

  return confirm_form(
    $form,
    t('Are your sure you want to delete the dashboard %name', array('%name' => $dashboard['name'])),
    'admin/dashboard/configure',
    t('This action cannot be undone.'),
    t('Delete')
	);
}

/**
 * Submit for confirm delete form for dashboards
 */
function nice_dash_dashboard_delete_form_submit($form, &$form_state) {
	nice_dash_remove_dashboard($form_state['values']['did']);
  drupal_set_message(t('The dashboard has been removed'));
  drupal_goto('admin/dashboard/configure');
}

function nice_dash_widget_settings_form(&$form_state, $wid = NULL){

	if(isset($wid)){
		$widget = nice_dash_widget_values($wid);
	}

	$form['title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title'),
		'#size' => 60,
		'#required' => TRUE,
		'#default_value' => $widget['title'],
	);

	$form['description'] = array(
		'#type' => 'textfield',
		'#title' => t('Description'),
		'#size' => 60,
		'#default_value' => $widget['description'],
	);

	$block_info = array();
  foreach (module_implements('block') as $module) {
    $module_blocks = module_invoke($module, 'block', 'list');
    if ($module_blocks) {
      foreach ($module_blocks as $delta => $info) {
        $block_info["{$module}-{$delta}"] = $info;
      }
    }
  }

  // Get default block options.
  $options = array();
  foreach ($block_info as $bid => $info) {
    if (!empty($enabled[$bid])) {
      $options[$bid] = $info['info'];
    }
  }
  asort($options);

  $result = db_query("SELECT name,type,info FROM {system} WHERE type = 'module' AND status = 1");
  $modules = array();
  while ($row = db_fetch_object($result)) {
    $info = unserialize($row->info);
    $modules[$row->name] = isset($info['name']) ? $info['name'] : $row->name;
  }
  foreach (array_diff_key($block_info, $options) as $bid => $info) {
    $module = array_shift(explode('-', $bid));
    $module = isset($modules[$module]) ? $modules[$module] : $module;
    $custom_options[$module][$bid] = $info['info'];
  }
  $custom_options = $custom_options + array(-1 => '<'. t('Choose a block') .'>');
  ksort($custom_options);

	// Default value from block select
	if($widget['widget_key']){
		$block_default = $widget['widget_key'];
	}
	else {
		$block_default = -1;
	}

  $form['block'] = array(
		'#title' => t('Block'),
    '#type' => 'select',
    '#options' => $custom_options,
    '#default_value' => $block_default,
  );

  $form['submit']['#type'] = 'submit';

  if ($wid) {
    $form['submit']['#value'] = t('Save changes');
  }
  else {
    $form['submit']['#value'] = t('Add widget');
  }

	$form['wid'] = array('#type' => 'value', '#value' => $wid);
	$form['#redirect'] = 'admin/dashboard/configure';

	return $form;
}

function nice_dash_widget_settings_form_submit(&$form, &$form_state){
	$values = $form_state['values'];
	$update_keys = array();

	$widget = new stdClass();
	$widget->widget_key = $values['block'];
	$widget->title = $values['title'];
	$widget->description = $values['description'];
	$widget->custom = 1;

	if($values['wid']){
		$update_keys = array('wid');
		$widget->wid = $values['wid'];
	}

	drupal_write_record('nice_dash_widgets', $widget, $update_keys);

	//Succes
	drupal_set_message(t('The widget has been saved'));
}

/**
 * Widget delete form
 */
function nice_dash_widget_delete_form($form_state, $wid){
  $form = array();
  $form['wid'] = array('#value' => $wid, '#type' => 'value');

  return confirm_form(
    $form,
    t('Are your sure you want to delete the widget'),
    'admin/dashboard/configure',
    t('This action cannot be undone.'),
    t('Delete')
	);
}

/**
 * Submit for confirm delete form for widgets
 */
function nice_dash_widget_delete_form_submit($form, &$form_state) {
	nice_dash_remove_widget($form_state['values']['wid']);
  drupal_set_message(t('The widget has been removed'));
  drupal_goto('admin/dashboard/configure');
}