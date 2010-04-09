<?php

/**
 * Callback for the Nice Dash: unpublish widget
 */
function nice_dash_scheduler_widget_unpublish(){
  $output .= '<div class="admin-panel">';
  $output .= '<h3>'.t('Scheduled to unpublish').'</h3>';
  $output .= views_embed_view('nice_dash_scheduler','block_4');
  $output .= '</div>';

  return $output;

}

/**
 * Callback for the Nice Dash: publish widget
 */
function nice_dash_scheduler_widget_publish(){
  $output .= '<div class="admin-panel">';
  $output .= '<h3>'.t('Scheduled to publish').'</h3>';
  $output .= views_embed_view('nice_dash_scheduler','block_5');
  $output .= '</div>';

  return $output;
}