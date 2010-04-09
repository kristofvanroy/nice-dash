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
function theme_nice_dash_page($left, $right){
	$output = '<div class="admin clear-block">';
	$output .= '<div class="left clear-block">'. $left .'</div>';
	$output .= '<div class="right clear-block">'. $right .'</div>';
	$output .= '</div>';
  return $output;
}


/**
* Theme the re-ordering form
*/
function theme_nice_dash_settings_form(&$vars) {

  drupal_add_js('misc/tableheader.js');

  drupal_add_tabledrag('draggabledemo-sort', 'order', 'sibling', 'sort');
  $header = array('', 'title', 'sort');
  foreach (element_children($form) as $key) {
    // Add class to group weight fields for drag and drop.
    $form[$key]['sort']['#attributes']['class'] = 'sort';

    $row = array(''); //This is important. We need to start with an empty element for the drag handle.

    $row[] = drupal_render($form[$key]['title']);
    $row[] = drupal_render($form[$key]['sort']);
    $rows[] = array('data' => $row, 'class' => 'draggable'); //note the difference between $row and $rows
  }
  $output = theme('table', $header, $rows, array('id' => 'draggabledemo-sort'));
  $vars['regions'] = $regions;
  $output .= drupal_render($form);
  return $output;
}