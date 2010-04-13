<?php

function nice_dash_comments_widget_latest_comments(){

  return theme('nice_dash_component', t('Latest comments'), t('An overview of the latest comments'), views_embed_view('nice_dash_comments','block_1'));
}
