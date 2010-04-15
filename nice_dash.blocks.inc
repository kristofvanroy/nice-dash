<?php

/**
 * Dashboard block callback
 */
function nice_dash_dashboard_block(){

  $dashboards = nice_dash_dashboards();

  $links['default']['href'] = 'admin/dashboard';
  $links['default']['title'] = t('Open dashboard');


	if(count($dashboards) > 0){
    foreach($dashboards as $dashboard){
      $links['dash-'.$dashboard['did']]['href'] = 'admin/dashboard/view/'.$dashboard['did'];
      $links['dash-'.$dashboard['did']]['title'] = $dashboard['name'];
    }
  }

  return theme('links', $links, array('class' => 'menu drilldown-active-menu'));
}
