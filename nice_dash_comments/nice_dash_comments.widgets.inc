<?php

function nice_dash_comments_widget_latest_comments(){

				/*$output .= '<div class="admin-panel">';
				$output .= '<h3><strong>'.t('Latest comments').'</strong></h3>';
				$output .= '<p class="description">'.t('An overview of the latest comments').'</p>';
					$output .= '<dl class="admin-list">';

					// Get latest nodes
				  foreach (comment_get_recent() as $comment) {
            $output .= '<dt style="border-bottom:1px solid #ccc; padding: 2px;"><small>' . l($comment->subject, 'node/'. $comment->nid, array('fragment' => 'comment-'. $comment->cid)) .'<br />'. t('@time ago', array('@time' => format_interval(time() - $comment->timestamp))) .'</small></dt>';
          }

					$output .= '</dl>';
			  	$output .= '<p class="description" style="padding-top: 10px;">' . l(t('View all comments'), 'admin/content/comment') .' - '. l(t('View approval queue'), 'admin/content/comment/approval') . '</p>';
					$output .= '</div>';*/

  return 'LATEST COMMENTS';
}
