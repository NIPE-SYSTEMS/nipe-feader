#include <libgrss.h>

static int i;
static GMainLoop *loop;

static gboolean mark_time (gpointer useless)
{
	printf ("%d\n", i++);

	if (i >= 15)
		g_main_loop_quit (loop);

	return TRUE;
}

static void
print_items (GObject *source, GAsyncResult *res, gpointer useless)
{
	GList *items;
	GList *iter;
	GError *error;
	GrssFeedChannel *channel;
	GrssFeedItem *it;

	error = NULL;
	channel = GRSS_FEED_CHANNEL (source);
	items = grss_feed_channel_fetch_all_finish (channel, res, &error);

	if (items == NULL) {
		printf ("%s\n", error->message);
	}
	else {
		for (iter = items; iter; iter = g_list_next (iter)) {
			it = (GrssFeedItem*) iter->data;
			printf ("%s\n", grss_feed_item_get_title (it));
		}
	}
}

static gboolean do_work (gpointer useless)
{
	GrssFeedChannel *feed;

	i = 0;
	g_timeout_add (100, mark_time, NULL);

	feed = grss_feed_channel_new_with_source ("http://www.nipe-systems.de/blog/rss.php");
	grss_feed_channel_fetch_all_async (feed, print_items, NULL);

	return FALSE;
}

int main ()
{
	loop = g_main_loop_new (NULL, FALSE);
	g_idle_add (do_work, NULL);

	g_main_run (loop);
	exit (0);
}