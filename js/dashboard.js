(function($) {

// Define settings
Drupal.settings.NiceDash = Drupal.settings.NiceDash || {toggleText: Drupal.t('Toggle filter')};

// Define main behavior
Drupal.behaviors.NiceDash = function(context) {
  $('.view-filters:not(.nicedash-processed)', context)
  .addClass('nicedash-processed')
  .each(function(i, el) {
    Drupal.behaviors.NiceDash.init(this);
  });
};

// Initialisation
Drupal.behaviors.NiceDash.init = function(elem) {
  var $filters = $(elem),
      $link = $('<a href="#" class="toggle-link">' + Drupal.settings.NiceDash.toggleText + '</a>')
      .bind('click', Drupal.behaviors.NiceDash.toggleFilters)
      .wrap('<div class="filter-container"></div>')
      .parent();
  $filters
  .before($link)
  .toggle();
};

// Click handler
Drupal.behaviors.NiceDash.toggleFilters = function() {
  $(this)
  .parent()
  .siblings('.view-filters')
  .slideToggle();
  return false;
};

})(jQuery);
