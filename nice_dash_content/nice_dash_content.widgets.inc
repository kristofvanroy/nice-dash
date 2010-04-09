<?php

/*
 * Callback for the Nice Dash: add content
 */

function nice_dash_content_widget_add_content(){
  $item = menu_get_item('node/add');
  $content = system_admin_menu_block($item);
  $output = theme('node_add_list', $content);

  return theme('nice_dash_component', t('Create content'), t('Select your content type'), $output);

}

/*
 * Callback for the Nice Dash: latest content
 */
function nice_dash_content_widget_latest_content() {

  return theme('nice_dash_component', t('Latest content'), t('An overview of the latest content'), views_embed_view('nice_dash_content','block_1'));

}

/*
 * Callback for the Nice Dash: sticky widget
 */
function nice_dash_content_widget_sticky() {

  return theme('nice_dash_component', t('Sticky items'), t('An overview of all content set as sticky'), views_embed_view('nice_dash_content','block_2'));

}

/*
 * Callback for the Nice Dash: sticky widget
 */
function nice_dash_content_widget_promoted() {

  return theme('nice_dash_component', t('Promoted to frontpage'), t('An overview of all content promoted to frontpage'), views_embed_view('nice_dash_content','block_3'));

}