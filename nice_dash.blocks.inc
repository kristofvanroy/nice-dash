<?php

/**
 * Dashboard block callback
 */
function nice_dash_dashboard_block(){

  $dashboards = nice_dash_dashboards();

	if(count($dashboards) > 0){
    foreach($dashboards as $dashboard){
      $links['dash-'.$dashboard['did']]['href'] = 'admin/dashboard/view/'.$dashboard['did'];
      $links['dash-'.$dashboard['did']]['title'] = $dashboard['name'];
    }
  }

  $links['create']['href'] = 'admin/dashboard/create-dashboard';
  $links['create']['title'] = t('Create new dashboard');

  $links['configure']['href'] = 'admin/dashboard/configure';
  $links['configure']['title'] = t('Configure dashboards');

  return theme('links', $links, array('class' => 'menu drilldown-active-menu'));
}
