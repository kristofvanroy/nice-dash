<?php

/**
 * @file
 * User plugin for Nice dashboard.
 */

class nice_dash_user {

  /**
   * Enabled method.
   */
  function is_enabled() {
    return TRUE;
  }

  /**
   * Views api method.
   */
  function views_api() {
    return TRUE;
  }

  /**
   * Widgets method.
   */
  function widgets() {
    return array(
      'user_latest' => t('Latest registered users'),
      'user_last_login' => t('Latest logged in users'),
    );
  }

  /**
   * Latests registered users.
   */
  function nice_dash_user_widget_user_latest() {
    return theme('nice_dash_component', t('Latest users'), t('An overview of the last registered users.'), views_embed_view('nice_dash_user', 'block_1'));
  }

  /**
   * Latests logged in users.
   */
  function nice_dash_user_widget_user_last_login() {
    return theme('nice_dash_component', t('Last loggedin users'), t('An overview of the users that have loggedin lately.'), views_embed_view('nice_dash_user', 'block_2'));
  }
}