<?php

/**
 * @file
 * Component template file.
 */
?>

<div id="<?php print $ctools_id; ?>" class="dashboard-component admin-panel ctools-collapsible-container ctools-collapsible-remember">
  <div class="ctools-collapsible-handle">
    <h3><strong><?php print $title; ?></strong></h3>
    <?php if ($description): ?>
      <p class="description"><?php print $description; ?></p>
    <?php endif; ?>
  </div>

  <div class="content ctools-collapsible-content">
    <?php print $content; ?>
  </div>
</div>
