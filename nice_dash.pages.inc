<?php

/**
 * Menu callback for the dashboard page.
 */
function nice_dash_dashboard_page() {
	$output = '';

	$widgets = module_invoke_all('nice_dash_widgets');

	if($widgets){
		foreach($widgets as $key => $value){
			$left .= call_user_func($value['callback']);
			$right .= call_user_func($value['callback']);
		}
	}

	return theme('nice_dash_page', $left, $right);

}

/**
 * Menu callback for the dashboard page.
 */
function nice_dash_dashboard_settings_form() {

	$form = array();

	$widgets = module_invoke_all('nice_dash_widgets');

  $form = array();
  $form['list']['#tree'] = TRUE;
  $form['list']['#theme'] = 'nice_dash_settings_form';

	if($widgets){
		foreach($widgets as $key => $value){
			$form['list'][$key]['title'] = array('#value' => $value['title']);
			$form['list'][$key]['sort'] = array('#type' => 'weight', '#delta' => 1, '#default_value' => $key);
		}
	}

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );


	return $form;

}

function nice_dash_dashboard_settings_form_submit(&$form, &$form_state){
	dpm($form_state['values']);
}

