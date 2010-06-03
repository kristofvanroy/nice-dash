<?php

/**
 *
 * TODO: CLEAN UP THIS API
 * - Duplicate functionalities
 *
 */

/**
 * Dashboard regions
 *
 * @return: An array of layout regions available for widgets
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

/**
 * Fetch widget values
 */
function nice_dash_widget_values($wid, $did = NULL){

 // If dashboard ID is supplied only give back dashboard region and weight
 if($did){
  $sql = 'SELECT region, weight FROM {nice_dash_config} WHERE wid = %d AND did = %d';
 }
 else {
	$sql = 'SELECT * FROM {nice_dash_widgets} WHERE wid = %d';
 }

 $result = db_query($sql, $wid, $did);
 while($row = db_fetch_array($result)){
   //if no region set to disabled
   if (!$row['region']) {
     $row['region'] = 'disabled';
   }
 
   return $row;
 }
 
 // If empty
 return nice_dash_widget_default_values();
}

/**
 * Fetch all widgets
 */
function nice_dash_widgets(){

 // @TODO add caching !!!!


 // Fetch code widgets
 $widgets = module_invoke_all('nice_dash_widgets');

 // Save them if they don't exist
 foreach($widgets as $key => $value){

	 $wid = nice_dash_widget_wid($key);

	 if(!$wid){
		$widget = new stdClass();
		$widget->custom = 0;
		$widget->widget_key = $key;
		$widget->title = $value['title'];
		$widget->description = '';

		drupal_write_record('nice_dash_widgets', $widget);
	 }
 }

 // Fetch db widgets
 $result = db_query("SELECT * FROM {nice_dash_widgets}");
 $widgets = array();
 while($row = db_fetch_array($result)){
	$widgets[] = $row;
 }
 return $widgets;
}

/**
 * Fetch all the custom widgets
 */
function nice_dash_custom_widgets(){
 $result = db_query("SELECT * FROM {nice_dash_widgets} WHERE custom = 1");

 $widgets = array();
 while($row = db_fetch_array($result)){
	 $widgets[] = $row;
 }
 return $widgets;
}

/**
 * Check if a widget key exists in the db
 */
function nice_dash_widget_wid($key){
 return db_result(db_query("SELECT wid FROM {nice_dash_widgets} WHERE widget_key = '%s'", $key));
}

/**
 * Default widget values
 */
function nice_dash_widget_default_values(){
 return array(
   'weight' => 0,
   'region' => 'disabled'
 );
}

/**
 * Delete widget
 */
function nice_dash_remove_widget($wid){
	db_query("DELETE FROM {nice_dash_config} WHERE wid = %d", $wid);
	db_query("DELETE FROM {nice_dash_widgets} WHERE wid = %d", $wid);
}

/**
 * Get the custom widget block
 */
function nice_dash_get_custom_widget($widget){
  $bid = $widget['widget_key'];
  $split = explode('-', $bid, 2);
  if (count($split) === 2) {
		 list($module, $delta) = $split;
		 $block = module_invoke($module, 'block', 'view', $delta);
		 if (!empty($block['content'])) {
			 if ($module === 'block') {
				 global $theme;
				 $block['subject'] = db_result(db_query("SELECT title FROM {blocks} WHERE module = 'block' AND delta = '%s'", $delta));
			 }
		 }
	}
	return theme('nice_dash_component', $widget['title'], $widget['description'], $block['content']);
}

/**
 * Fetch dashboards
 */
function nice_dash_dashboard_values($did){
	$result = db_query("SELECT * FROM {nice_dash_dashboards} WHERE did = %d", $did);
	$rows = array();

	while ($row = db_fetch_array($result)){
		return $row;
	}
}

/**
 * Fetch dashboards
 */
function nice_dash_dashboards(){
	$result = db_query("SELECT * FROM {nice_dash_dashboards} ORDER BY weight ASC");
	$rows = array();

	while ($row = db_fetch_array($result)){
	 $rows[] = $row;
	}

	return $rows;
}

/**
 * Delete dashboard
 */
function nice_dash_remove_dashboard($did){
	db_query("DELETE FROM {nice_dash_dashboards} WHERE did = %d", $did);
	db_query("DELETE FROM {nice_dash_config} WHERE did = %d", $did);
	// Clear them from the menu
	module_invoke('menu','rebuild');
}

