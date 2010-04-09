<?php

/**
 * Dashboard block callback
 */
function nice_dash_dashboard_block(){

  $output['goto']['href'] = 'admin/dashboard';
  $output['goto']['title'] = t('Open dashboard');

  return theme('links', $output, array('class' => 'menu drilldown-active-menu'));
}
