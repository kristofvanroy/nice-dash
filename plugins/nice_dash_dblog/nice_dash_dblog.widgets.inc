<?php

/**
 * Callback for the Nice Dash: unpublish widget
 */
function nice_dash_dblog_widget_dblog(){
  return theme('nice_dash_component', t('Latest log entries'), t('An overview of the log entries'), views_embed_view('nice_dash_dblog','block_1'));

}