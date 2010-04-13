<?php

/**
 * Callback for the Nice Dash: unpublish widget
 */
function nice_dash_user_widget_latest(){

  return theme('nice_dash_component', t('Latest users'), t('An overview of the last registered users.'), views_embed_view('nice_dash_user','block_1'));

}

function nice_dash_user_widget_last_login(){

  return theme('nice_dash_component', t('Last loggedin users'), t('An overview of the users that have loggedin lately.'), views_embed_view('nice_dash_user','block_2'));

}
