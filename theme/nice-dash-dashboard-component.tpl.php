<div class="dashboard-component admin-panel">
  <h3><strong><?php print $title; ?></strong></h3>
  <?php if ($description): ?>
    <p class="description"><?php print $description; ?></p>
  <?php endif; ?>

  <div class="content">
    <?php print $content; ?>
  </div>
</div>
