<?php

/**
 * @file
 * Menu callbacks for Nice dashboard.
 */

/**
 * Settings for Nice dash.
 */
function nice_dash_settings($form_state) {
  $form = array();

  $form['nice_dash_redirect_after_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect after login'),
    '#description' => t('Toggle this checkbox to redirect to the dashboard after login.'),
    '#default_value' => variable_get('nice_dash_redirect_after_login', FALSE),
  );

  $form['nice_dash_alter_admin_path'] = array(
    '#type' => 'checkbox',
    '#title' => t('Take over admin path'),
    '#description' => t('Toggle this checkbox to take over the admin path.'),
    '#default_value' => variable_get('nice_dash_alter_admin_path', FALSE),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'nice_dash_settings_submit';
  return $form;
}

/**
 * Submit function for settings form.
 */
function nice_dash_settings_submit($form, &$form_state) {
  module_invoke('menu', 'rebuild');
}

/**
 * Menu callback for the dashboard page.
 */
function nice_dash_dashboard_page($did = NULL) {

  $regions = array();
  $widgets = nice_dash_get_object('widget');
  if ($widgets) {

    // Fetch the default dashboard.
    if (!$did) {
      $did = db_result(db_query("SELECT did FROM {nice_dash_dashboards} ORDER BY weight ASC LIMIT 1"));
    }

    $result = db_query("SELECT * FROM {nice_dash_config} c
                        LEFT JOIN {nice_dash_widgets} w  ON c.wid = w.wid
                        WHERE did = %d
                        ORDER BY weight", $did);

    while ($row = db_fetch_object($result)) {
      $widget = '';

      if ($row->region != 'disabled') {
        if ($row->custom) {
          $object = nice_dash_get_plugin('nice_dash');
          $widget = $object->nice_dash_widget_custom($row);
        }
        else {
          $method = $row->widget_key;
          $plugin = $row->widget_plugin;
          $method_name = $plugin .'_widget_'. $method;
          $object = nice_dash_get_plugin($plugin);
          if (is_object($object) && method_exists($object, $method_name)) {
            $widget = $object->$method_name();
          }
        }

        if (!empty($widget)) {
          if (!isset($regions[$row->region])) $regions[$row->region] = '';
          $widget_output = theme('nice_dash_component', $widget['title'], isset($widget['description']) ? $widget['description'] : '', $widget['content']);
          $regions[$row->region] .= $widget_output;
        }
      }
    }
  }

  if (!count($regions)) {
    drupal_set_message(t('You have not configured any widgets yet. Goto the <a href="@settings-page">dashboard settings</a> and configure some widgets first.', array('@settings-page' => url('admin/dashboard/configure/' . $did . '/edit'))), 'warning');
    return '';
  }

  return theme('nice_dash_page', $regions);

}

/**
 * Overview page with the dashboards and custom widgets.
 */
function nice_dash_dashboard_overview_page() {

  $form = array();

  $form['#tree'] = TRUE;
  $form['#theme'] = 'nice_dash_overview';

  $dashboards = nice_dash_get_object();
  $output = '<h2>'. t('Dashboards') .'</h2>';

  if (count($dashboards) > 0) {

    $form = array();
    $form['#tree'] = TRUE;
    $form['#theme'] = 'nice_dash_overview';

    $dashboards = nice_dash_get_object();
    $output = '<h2>'. t('Dashboards') .'</h2>';

    $headers = array(t('Name'), t('Actions'));
    $rows = array();
    foreach ($dashboards as $dashboard) {
      $form['#dashboards'][$dashboard->did] = $dashboard->weight;
      $form[$dashboard->did]['title'] = array('#value' => $dashboard->name);
      $form[$dashboard->did]['did'] = array('#value' => $dashboard->did);
      $form[$dashboard->did]['weight'] = array(
        '#type' => 'weight',
        '#delta' => 10,
        '#default_value' => $dashboard->weight,
        '#attributes' => array(
          'class' => 'overview-weight'
        )
      );
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

  }
  else {
    drupal_set_message(t('You have to create a dashboard first. You can create your first dashboard on the <a href="@create-page">dashboard create page</a>.', array('@create-page' => url("admin/dashboard/configure/create-dashboard"))), 'warning');
    $output .= '<p>'. t('No dashboards configured.') .'</p>';
  }

  return $form;
}

/**
 * Submit for the dashboard overview form.
 */
function nice_dash_dashboard_overview_page_submit(&$form, &$form_state) {

  foreach($form['#dashboards'] as $did => $weight) {
    $object = new StdClass();
    $object->weight = $form_state['values'][$did]['weight'];
    $object->did = $form[$did]['did']['#value'];
    drupal_write_record('nice_dash_dashboards', $object, 'did');
  }
  nice_dash_flush_caches();
  menu_rebuild();
  drupal_set_message(t('Your settings have been saved.'));
}

/**
 * Menu callback for the dashboard settings page.
 */
function nice_dash_dashboard_settings_form(&$form_state, $did = NULL) {
  $form = array();

  if (isset($did)) {
    $dashboard = nice_dash_get_object('dashboard', $did);
  }
  $widgets = nice_dash_get_object('widget');

  if ($widgets) {

    $form['#tree'] = TRUE;
    $form['#theme'] = 'nice_dash_settings_form';

    foreach ($widgets as $widget) {
      // Fetch values for the widget.
      $default_values = _nice_dash_widget_values($widget->wid, $did);

      $form['#widgets'][] = $widget->wid;
      $form[$widget->wid]['title'] = array('#value' => $widget->title);
      $form[$widget->wid]['region'] = array(
        '#type' => 'select',
        '#options' => nice_dash_regions(),
        '#default_value' => $default_values['region'],
        '#attributes' => array(
          'class' => 'field-region-select field-region-disabled ',
        ),
      );
      $form[$widget->wid]['weight'] = array('#type' => 'weight', '#delta' => 1, '#default_value' => $default_values['weight']);
    }

    $form['widgets'] = array('#type' => 'value', '#value' => $form['#widgets']);

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }
  else {
    $form = array();
    drupal_set_message(t('No widgets found.'), 'warning');
  }

  $form['dashboard_name'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($dashboard->name) ? $dashboard->name : '',
    '#title' => t('Dashboard name'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $url_alias = '';
  if (isset($dashboard->did)) {
    $internal_path = 'admin/dashboard/view/'. $dashboard->did;
    $db_alias = drupal_get_path_alias($internal_path);
    if ($internal_path != $db_alias) {
      $url_alias = $db_alias;
    }
  }
  $form['dashboard_alias'] = array(
    '#type' => 'textfield',
    '#default_value' => str_replace('dashboard/', '', $url_alias),
    '#title' => t('URL alias'),
    '#size' => 60,
    '#maxlength' => 128,
    '#description' => t('The prefix will be \'dashboard/\'. All characters will be set to lowercase and spaces will be rewritten to hyphens.')
  );

  $form['dashboard_access'] = array(
    '#type' => 'checkboxes',
    '#default_value' => $dashboard->access == '' ? '' : unserialize($dashboard->access),
    '#title' => t('Dashboard access'),
    '#options' => user_roles(TRUE)
  );

  // Current dashboard id.
  $form['dashboard_id'] = array('#type' => 'value', '#value' => $did);
  return $form;
}

/**
 * Fetch widget values.
 */
function _nice_dash_widget_values($wid, $did = NULL) {

  // If dashboard ID is supplied only give back dashboard region and weight.
  if ($did) {
    $sql = 'SELECT region, weight FROM {nice_dash_config} WHERE wid = %d AND did = %d';
  }
  else {
    $sql = 'SELECT * FROM {nice_dash_widgets} WHERE wid = %d';
  }

  $result = db_query($sql, $wid, $did);
  while ($row = db_fetch_array($result)) {

    // If no region set to disabled.
    if (!$row['region']) {
      $row['region'] = 'disabled';
    }

    return $row;
  }

  // Return default values when empty.
  return array(
   'weight' => 0,
   'region' => 'disabled'
  );
}

/**
 * Settings form validate handler.
 */
function nice_dash_dashboard_settings_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  $did = $values['dashboard_id'];
  $dashboard_name = $values['dashboard_name'];

  $query_values = array($dashboard_name);
  $query = "SELECT name FROM {nice_dash_dashboards} WHERE name = '%s'";

  if (!empty($did)) {
    $query .= ' AND did != %d';
    $query_values[] = $did;
  }

  if (db_result(db_query($query, $query_values))) {
    form_set_error('dashboard_name', t('Dashboard name %dashboard_name already exists.', array('%dashboard_name' => $dashboard_name)));
  }

  if (!preg_match('!^[ A-Za-z0-9_-]+$!', $dashboard_name)) {
    form_set_error('dashboard_name', t('The dashboard name must contain only uppercase and lowercase letters, underscores or hyphens.'));
  }
}

/**
 * Settings form submit handler.
 */
function nice_dash_dashboard_settings_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];

  // Dashboard
  $update_keys = array();
  $dashboard = new stdClass();
  $dashboard->name = $values['dashboard_name'];

  // Access.
  $dashboard->access = '';
  foreach($values['dashboard_access'] as $value) {
    if ($value != 0) {
      $dashboard->access = serialize($values['dashboard_access']);
      break;
    }
  }

  // Update or not.
  if (isset($values['dashboard_id']) && !empty($values['dashboard_id'])) {
    $update_keys = array('did');
    $dashboard->did = $values['dashboard_id'];
  }
  else {
    $weight = db_result(db_query("SELECT MAX(weight) FROM {nice_dash_dashboards}"));
    $dashboard->weight = $weight + 1;
  }

  // Save dashboard.
  drupal_write_record('nice_dash_dashboards', $dashboard, $update_keys);

  // Widgets
  if (sizeof($values['widgets']) > 0) {
    foreach ($values['widgets'] as $wid) {

      $widget = new stdClass();
      $update_keys = array();

      // Check if row exists
      $db_wid = db_result(db_query("SELECT wid FROM {nice_dash_config} WHERE wid = %d AND did = %d", $wid, $dashboard->did));

      $widget->wid = $wid;
      $widget->did = $dashboard->did;
      $widget->region = $values[$wid]['region'];
      $widget->weight = $values[$wid]['weight'];

      if (is_numeric($db_wid)) {
        $widget->wid = $db_wid;
        $update_keys = array('wid', 'did');
      }

      // Save widget.
      drupal_write_record('nice_dash_config', $widget, $update_keys);
    }
  }

  // Path alias.
  if (!empty($values['dashboard_alias'])) {
    $alias = $values['dashboard_alias'];
    if (module_exists('pathauto')) {
      _pathauto_include();
      $alias = pathauto_cleanstring($alias);
    }
    else {
      $alias = str_replace(' ', '-', $alias);
      $alias = strtolower($alias);
    }
    $alias = 'dashboard/'. $alias;
    path_set_alias('admin/dashboard/view/'. $dashboard->did, $alias);
  }
  else {
    path_set_alias('admin/dashboard/view/'. $dashboard->did);
  }

  // Rebuild menu.
  nice_dash_flush_caches();
  module_invoke('menu', 'rebuild');

  // Success
  drupal_set_message(t('Saved dashboard.'));

}

/**
 * Confirm delete form for dashboards
 */
function nice_dash_dashboard_delete_form($form_state, $did) {
  $form = array();
  $form['did'] = array('#value' => $did, '#type' => 'value');
  $dashboard = nice_dash_get_object('dashboard', $did);

  if (empty($dashboard)) {
    drupal_set_message(t('The dashboard you want to delete does not exist. Go back to the <a href="@overview-page">dashboard overview page</a>.', array('@overview-page' => url("admin/dashboard/configure"))), 'warning');
    return $form;
  }

  return confirm_form(
    $form,
    t('Are your sure you want to delete the dashboard %name', array('%name' => $dashboard->name)),
    'admin/dashboard/configure',
    t('This action cannot be undone.'),
    t('Delete')
  );
}

/**
 * Submit for confirm delete form for dashboards.
 */
function nice_dash_dashboard_delete_form_submit($form, &$form_state) {
  $did = $form_state['values']['did'];
  db_query("DELETE FROM {nice_dash_dashboards} WHERE did = %d", $did);
  db_query("DELETE FROM {nice_dash_config} WHERE did = %d", $did);
  db_query("DELETE FROM {url_alias} WHERE src = '%s'", 'admin/dashboard/view/'. $did);

  // Rebuild menu.
  nice_dash_flush_caches();
  module_invoke('menu', 'rebuild');

  drupal_set_message(t('The dashboard has been removed'));
  drupal_goto('admin/dashboard/configure');
}

/**
 * Widget settings form.
 */
function nice_dash_widget_settings_form(&$form_state, $wid = NULL) {

  if (isset($wid)) {
    $widget = nice_dash_get_object('widget', $wid);
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

  // Default value from block select.
  if ($widget['widget_key']) {
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

/**
 * Widget submit save form.
 */
function nice_dash_widget_settings_form_submit(&$form, &$form_state) {
  $update_keys = array();
  $values = $form_state['values'];

  $widget = new stdClass();
  $widget->widget_key = $values['block'];
  $widget->title = $values['title'];
  $widget->description = $values['description'];
  $widget->custom = 1;

  if ($values['wid']) {
    $update_keys = array('wid');
    $widget->wid = $values['wid'];
  }

  drupal_write_record('nice_dash_widgets', $widget, $update_keys);
  nice_dash_flush_caches();

  // Succes.
  drupal_set_message(t('The widget has been saved'));
}

/**
 * Widget delete form.
 */
function nice_dash_widget_delete_form($form_state, $wid) {
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
 * Submit for confirm delete form for widgets.
 */
function nice_dash_widget_delete_form_submit($form, &$form_state) {
  $wid = $form_state['values']['wid'];
  db_query("DELETE FROM {nice_dash_config} WHERE wid = %d", $wid);
  db_query("DELETE FROM {nice_dash_widgets} WHERE wid = %d", $wid);
  nice_dash_flush_caches();
  drupal_set_message(t('The widget has been removed'));
  drupal_goto('admin/dashboard/configure');
}

/**
 * Function to return an array of layout regions available for widgets.
 */
function nice_dash_regions() {
  return array(
    'header' => t('Header'),
    'left' => t('Left'),
    'right' => t('Right'),
    'footer' => t('Footer'),
    'disabled' => t('Disabled')
  );
}

