#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
	FILE * wget = NULL;
	FILE * fb2rss = NULL;
	FILE * git = NULL;
	FILE * diff = NULL;
	char * s = malloc(sizeof(*s)*256);
	int d;
	diff = popen("touch posts.xml","r");
	pclose(diff);
	diff = popen("touch posts.1.xml", "r");
	while (1) {
		wget = popen("wget -U \"Mozzila/5.0\" \"https://www.facebook.com/1stawesomeplatoon/posts\" -Oposts.html", "r");
		if (!wget) {
			fprintf(stderr, "error: wget isn't available\n");
			return 0;
		}
		pclose(wget);

		fprintf(stderr,"converting...");
		fb2rss = popen("./fb2rss posts.html -oposts.1.xml", "r");
		if (!fb2rss) {
			fprintf(stderr, "error: fb2rss isn't available\n");
			return 0;
		}
		pclose(fb2rss);

		diff = popen("diff posts.1.xml posts.xml", "r");
		if (!diff) {
			fprintf(stderr, "error: diff isn't available\n");
			return 0;
		}
		d = 0;
		while (fgets(s,256,diff)) {
			d++;
		}
		pclose(diff);
		if (d > 5) {

			diff = popen("mv posts.1.xml posts.xml","r");
			pclose(diff);

			fprintf(stderr,"commiting...");
			git = popen("git commit -a -m \"auto-commit by wrapper\"", "r");
			if (!git) {
				fprintf(stderr, "error: git isn't available\n");
				return 0;
			}
			pclose(git);

			fprintf(stderr,"pushing...");
			git = popen("git push", "r");
			if (!git) {
				fprintf(stderr, "error: git isn't available\n");
				return 0;
			}
			pclose(git);
			fprintf(stderr,"done!");
		}

		unsigned long now = time(0);
		strftime(s, 256, "%M", localtime(&now));
		int minutes = atoi(s);
		int wait_time = 30 - (minutes % 30) - 1;
		wait_time *= 60;
		strftime(s, 256, "%S", localtime(&now));
		wait_time -= atoi(s);
		fprintf(stderr,"waiting %d\n",wait_time);
		sleep(wait_time); //half an hour
	}

	return 0;
}
