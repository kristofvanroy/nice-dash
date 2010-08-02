Drupal.NiceDash = Drupal.NiceDash || {};

$(document).ready(function(){
  Drupal.NiceDash();
});

Drupal.NiceDash = function(){
  initFilters();
  function initFilters(){
    $('.view-filters').parent().before('<div class="filter-container"><a href="" class="toggle-link">Toggle filter</a></div><br class="clear" />');
    $('.view-filters').toggle();
    $('.toggle-link').bind('click', toggleFilters);
  }
  function toggleFilters(e){
    $(e.target).parent().parent().find('.view-filters').slideToggle();
    e.preventDefault();
  }
}