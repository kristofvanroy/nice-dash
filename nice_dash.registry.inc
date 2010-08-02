<?php

/**
 * @file
 * Registry file for Nice dashboard.
 * Contains functions which are only called after a cache clear.
 */


/**
 * Fetch dashboards.
 */
function _nice_dash_get_dashboards() {
  $rows = array();
  $result = db_query("SELECT * FROM {nice_dash_dashboards} ORDER BY weight ASC");

  while ($row = db_fetch_object($result)) {
    $rows[$row->did] = $row;
  }

  return $rows;
}

/**
 * Fetch all widgets.
 */
function _nice_dash_get_all_widgets() {

  // Fetch code widgets.
  foreach (nice_dash_get_plugin() as $plugin) {

    $object = nice_dash_get_plugin($plugin['name']);
    if (!$object->is_enabled() || !method_exists($object, 'widgets')) {
      continue;
    }

    $widgets = $object->widgets();
    foreach ($widgets as $key => $value) {

      $wid = _nice_dash_widget_wid($key);

      if (!$wid) {
        $widget = new stdClass();
        $widget->custom = 0;
        $widget->widget_key = $key;
        $widget->widget_plugin = $plugin['name'];
        $widget->title = $value;
        $widget->description = '';

        drupal_write_record('nice_dash_widgets', $widget);
      }
    }
  }

  // Fetch db widgets.
  $widgets = array();
  $result = db_query("SELECT * FROM {nice_dash_widgets}");
  while ($row = db_fetch_object($result)) {
    $widgets[$row->wid] = $row;
  }

  return $widgets;
}

/**
 * Helper function to check if a widget key exists in the db.
 */
function _nice_dash_widget_wid($key) {
  return db_result(db_query("SELECT wid FROM {nice_dash_widgets} WHERE widget_key = '%s'", $key));
}

/**
 * Fetch all custom widgets.
 */
function _nice_dash_get_custom_widgets() {
  $widgets = array();

  $result = db_query("SELECT * FROM {nice_dash_widgets} WHERE custom = 1");
  while ($row = db_fetch_object($result)) {
    $widgets[$row->wid] = $row;
  }

  return $widgets;
}

/**
 * Return menu items.
 */
function _nice_dash_menu() {

  $items['admin/dashboard/configure'] = array(
    'title' => 'Configure',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('nice_dash_dashboard_overview_page'),
    'access arguments' => array('administer nice dashboard'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'nice_dash.pages.inc'
  );


  $weight = 0;
  $items['admin/dashboard/configure/overview'] = array(
    'title' => 'Overview',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('nice_dash_dashboard_overview_page'),
    'access arguments' => array('administer nice dashboard'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => $weight++,
    'file' => 'nice_dash.pages.inc'
  );

  $items['admin/dashboard/configure/settings'] = array(
    'title' => 'Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('nice_dash_settings'),
    'access arguments' => array('administer nice dashboard'),
    'type' => MENU_LOCAL_TASK,
    'weight' => $weight++,
    'file' => 'nice_dash.pages.inc'
  );

  $items['admin/dashboard/configure/create-dashboard'] = array(
    'title' => 'Create dashboard',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('nice_dash_dashboard_settings_form'),
    'access arguments' => array('administer nice dashboard'),
    'type' => MENU_LOCAL_TASK,
    'weight' => $weight++,
    'file' => 'nice_dash.pages.inc'
  );

  $items['admin/dashboard/configure/%/delete'] = array(
    'title' => 'Create new dashboard',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('nice_dash_dashboard_delete_form', 3),
    'access arguments' => array('administer nice dashboard'),
    'type' => MENU_CALLBACK,
    'file' => 'nice_dash.pages.inc'
  );

  // Widgets.
  $items['admin/dashboard/configure/create-widget'] = array(
    'title' => 'Create widget',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('nice_dash_widget_settings_form'),
    'access arguments' => array('administer nice dashboard'),
    'type' => MENU_LOCAL_TASK,
    'weight' => $weight++,
    'file' => 'nice_dash.pages.inc'
  );

  $items['admin/dashboard/widget/%/delete'] = array(
    'title' => 'Delete widget',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('nice_dash_widget_delete_form', 3),
    'access arguments' => array('administer nice dashboard'),
    'type' => MENU_CALLBACK,
    'file' => 'nice_dash.pages.inc'
  );

  $items['admin/dashboard/widget/%/edit'] = array(
    'title' => 'Edit widget',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('nice_dash_widget_settings_form', 3),
    'access arguments' => array('administer nice dashboard'),
    'type' => MENU_CALLBACK,
    'file' => 'nice_dash.pages.inc'
  );

  // Implement all the dashboard menu calls.
  $dashboards = nice_dash_get_object();

  if (count($dashboards) > 0) {

    foreach ($dashboards as $dashboard) {
      $items['admin/dashboard/configure/'. $dashboard->did .'/edit'] = array(
        'title' => 'Edit '. $dashboard->name,
        'weight' => $dashboard->weight,
        'page callback' => 'drupal_get_form',
        'page arguments' => array('nice_dash_dashboard_settings_form', 3),
        'access arguments' => array('administer nice dashboard'),
        'type' => MENU_CALLBACK,
        'file' => 'nice_dash.pages.inc'
      );

      $items['admin/dashboard/view/'. $dashboard->did] = array(
        'title' => $dashboard->name,
        'page callback' => 'nice_dash_dashboard_page',
        'page arguments' => array(3),
        'access callback' => 'nice_dash_dashboard_access',
        'access arguments' => array($dashboard),
        'type' => MENU_LOCAL_TASK,
        'weight' => $dashboard->weight,
        'file' => 'nice_dash.pages.inc'
      );
    }
  }

  $items['admin/dashboard'] = array(
    'title' => 'Dashboard',
    'page callback' => '_nice_dash_dashboard_page',
    'access callback' => '_nice_dash_dashboard_access',
    'type' => MENU_NORMAL_ITEM,
    'file' => 'nice_dash.pages.inc'
  );

  $items['admin/dashboard/view'] = array(
    'title' => 'View',
    'page callback' => '_nice_dash_dashboard_page',
    'access callback' => '_nice_dash_dashboard_access',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -1,
    'file' => 'nice_dash.pages.inc'
  );

  return $items;
}

/**
 * Alter menu items.
 */
function _nice_dash_menu_alter(&$items) {
  if (variable_get('nice_dash_alter_admin_path', FALSE)) {
    $items['admin'] = array(
      'title' => 'Dashboard',
      'page callback' => '_nice_dash_dashboard_page',
      'access callback' => '_nice_dash_dashboard_access',
      'type' => MENU_NORMAL_ITEM,
      'file' => 'nice_dash.pages.inc',
      'file path' => drupal_get_path('module', 'nice_dash'),
    );
  }
}

/**
 * Return theming functions.
 */
function _nice_dash_theme() {
  $path = drupal_get_path('module', 'nice_dash');

  return array(
    'nice_dash_component' => array(
      'arguments' => array('title' => NULL, 'description' => NULL, 'content' => NULL),
      'template' => 'nice-dash-dashboard-component',
      'file' => 'theme.inc',
      'path' => $path .'/theme',
    ),
    'nice_dash_page' => array(
      'arguments' => array('regions' => NULL),
      'template' => 'nice-dash-dashboard',
      'file' => 'theme.inc',
      'path' => $path .'/theme',
    ),
    'nice_dash_settings_form' => array(
      'arguments' => array('form' => NULL),
      'template' => 'nice-dash-dashboard-settings-form',
      'file' => 'theme.inc',
      'path' => $path .'/theme',
    ),
    'nice_dash_overview' => array(
      'arguments' => array('form' => NULL),
      'template' => 'nice-dash-dashboard-overview',
      'file' => 'theme.inc',
      'path' => $path .'/theme',
    ),
  );
}

/**
 * Return plugins.
 */
function _nice_dash_nice_dash_plugins() {
  $plugins = array();

  $plugins['nice_dash'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'nice_dash') .'/plugins/nice_dash',
      'file' => 'nice_dash.inc',
      'class' => 'nice_dash',
    ),
  );
  $plugins['nice_dash_content'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'nice_dash') .'/plugins/nice_dash_content',
      'file' => 'nice_dash_content.inc',
      'class' => 'nice_dash_content',
    ),
  );
  $plugins['nice_dash_comments'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'nice_dash') .'/plugins/nice_dash_comments',
      'file' => 'nice_dash_comments.inc',
      'class' => 'nice_dash_comments',
    ),
  );
  $plugins['nice_dash_ga'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'nice_dash') .'/plugins/nice_dash_ga',
      'file' => 'nice_dash_ga.inc',
      'class' => 'nice_dash_ga',
    ),
  );
  $plugins['nice_dash_scheduler'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'nice_dash') .'/plugins/nice_dash_scheduler',
      'file' => 'nice_dash_scheduler.inc',
      'class' => 'nice_dash_scheduler',
    ),
  );
  $plugins['nice_dash_user'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'nice_dash') .'/plugins/nice_dash_user',
      'file' => 'nice_dash_user.inc',
      'class' => 'nice_dash_user',
    ),
  );
  return $plugins;
}

