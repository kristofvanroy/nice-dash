<?php

/**
 * Callback for the Nice Dash: unpublish widget
 */
function nice_dash_user_widget_latest(){
  $output .= '<div class="admin-panel">';
  $output .= '<h3>'.t('Latest registered users').'</h3>';
  $output .= views_embed_view('nice_dash_user','block_1');
  $output .= '</div>';

  return $output;

}
