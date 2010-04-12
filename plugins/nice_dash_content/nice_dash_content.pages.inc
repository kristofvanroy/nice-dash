<?php

/**
 * Menu callback for the demote action
 */
function nice_dash_dashboard_demote($nid) {
	$node = node_load($nid);

	if ($node) {
		db_query("UPDATE {node} SET promote = '0' WHERE nid = %d", $node->nid);
		drupal_set_message('The ' . $node->type . ' ' . $node->title . ' was demoted successfully.');
	}

	drupal_goto('admin/dashboard');
}

/**
 * Menu callback for the unsticky action
 */
function nice_dash_dashboard_unsticky($nid) {
	$node = node_load($nid);

	if ($node) {
		db_query("UPDATE {node} SET sticky = '0' WHERE nid = %d", $node->nid);
		drupal_set_message('The ' . $node->type . ' ' . $node->title . ' was unset as sticky successfully.');
	}

	drupal_goto('admin/dashboard');
}