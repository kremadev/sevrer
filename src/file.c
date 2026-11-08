#include "file.h"

static const struct {
	const char *ext;
	const char *type;
} mime_types[] = {
	/* text */
	{ ".html",  "text/html; charset=utf-8" },
	{ ".htm",   "text/html; charset=utf-8" },
	{ ".css",   "text/css; charset=utf-8" },
	{ ".js",    "text/javascript; charset=utf-8" },
	{ ".mjs",   "text/javascript; charset=utf-8" },
	{ ".txt",   "text/plain; charset=utf-8" },
	{ ".md",    "text/markdown; charset=utf-8" },
	{ ".csv",   "text/csv; charset=utf-8" },
	{ ".xml",   "application/xml" },
	{ ".json",  "application/json" },
	{ ".c",     "text/plain; charset=utf-8" },
	{ ".h",     "text/plain; charset=utf-8" },
	{ ".asm",   "text/plain; charset=utf-8" },
	{ ".s",     "text/plain; charset=utf-8" },
	{ ".sh",    "text/plain; charset=utf-8" },
	{ ".py",    "text/plain; charset=utf-8" },
	{ ".log",   "text/plain; charset=utf-8" },
	{ ".conf",  "text/plain; charset=utf-8" },

	/* images */
	{ ".png",   "image/png" },
	{ ".jpg",   "image/jpeg" },
	{ ".jpeg",  "image/jpeg" },
	{ ".gif",   "image/gif" },
	{ ".webp",  "image/webp" },
	{ ".avif",  "image/avif" },
	{ ".svg",   "image/svg+xml" },
	{ ".ico",   "image/x-icon" },
	{ ".bmp",   "image/bmp" },
	{ ".tif",   "image/tiff" },
	{ ".tiff",  "image/tiff" },
	{ ".ppm",   "image/x-portable-pixmap" },

	/* audio */
	{ ".wav",   "audio/wav" },
	{ ".mp3",   "audio/mpeg" },
	{ ".ogg",   "audio/ogg" },
	{ ".oga",   "audio/ogg" },
	{ ".opus",  "audio/ogg" },
	{ ".flac",  "audio/flac" },
	{ ".m4a",   "audio/mp4" },
	{ ".aac",   "audio/aac" },
	{ ".mid",   "audio/midi" },
	{ ".midi",  "audio/midi" },

	/* video */
	{ ".mp4",   "video/mp4" },
	{ ".m4v",   "video/mp4" },
	{ ".webm",  "video/webm" },
	{ ".ogv",   "video/ogg" },
	{ ".mov",   "video/quicktime" },
	{ ".mkv",   "video/x-matroska" },
	{ ".avi",   "video/x-msvideo" },

	/* fonts */
	{ ".woff",  "font/woff" },
	{ ".woff2", "font/woff2" },
	{ ".ttf",   "font/ttf" },
	{ ".otf",   "font/otf" },

	/* documents */
	{ ".pdf",   "application/pdf" },

	/* archives / binaries */
	{ ".zip",   "application/zip" },
	{ ".gz",    "application/gzip" },
	{ ".tar",   "application/x-tar" },
	{ ".xz",    "application/x-xz" },
	{ ".zst",   "application/zstd" },
	{ ".7z",    "application/x-7z-compressed" },
	{ ".iso",   "application/x-iso9660-image" },
	{ ".bin",   "application/octet-stream" },
	{ ".wasm",  "application/wasm" },
};

static const char *doc_root = ".";

void file_set_root(const char *root) {
	doc_root = root;
}

static const char *get_content_type(const char *path) {
	const char *file_extension = strrchr(path, '.');
	if (file_extension == NULL) return "application/octet-stream";

	for (size_t i = 0; i < sizeof(mime_types) / sizeof(mime_types[0]); i++) {
		if (!strcasecmp(file_extension, mime_types[i].ext))
			return mime_types[i].type;
	}
	return "application/octet-stream";
}

static char *not_found(int *size) {
	const char *body = "<!DOCTYPE html><html><head><title>Sevrer - 404 Not Found</title></head><body><center><h1>404 Not Found</h1><hr size=\"2\" width=\"100%\"><p>404 Not Found - Try <a href=\"/index.html\">/index.html</a></p></center></body></html>";
	int body_len = strlen(body);

	char header[256];
	int header_len = snprintf(header, sizeof(header),
		"HTTP/1.1 404 Not Found\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: %d\r\nConnection: close\r\n\r\n",
		body_len);

	char *buffer = malloc(header_len + body_len);
	if (buffer == NULL) return NULL;

	memcpy(buffer, header, header_len);
	memcpy(buffer + header_len, body, body_len);
	*size = header_len + body_len;
	return buffer;
}

char *get_file_content(char *path, int *size) {
	if (path[0] != '/' || strstr(path, "..")) return not_found(size);

	char *query = strpbrk(path, "?#");
	if (query) *query = '\0';

	const char *rel = path;
	if (!strcmp(path, "/")) rel = "/index.html";

	size_t full_len = strlen(doc_root) + strlen(rel) + 1;
	char *full = malloc(full_len);
	if (full == NULL) return NULL;
	snprintf(full, full_len, "%s%s", doc_root, rel);

	FILE *file = fopen(full, "rb");
	free(full);
	if (file == NULL) return not_found(size);

	struct stat st;
	if (fstat(fileno(file), &st) != 0 || !S_ISREG(st.st_mode)) {
		fclose(file);
		return not_found(size);
	}
	long file_size = st.st_size;

	const char *content_type = get_content_type(rel);

	char header[256];
	int header_len = snprintf(header, sizeof(header),
		"HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: %s\r\nConnection: close\r\n\r\n",
		file_size, content_type);

	char *buffer = malloc(header_len + file_size);
	if (buffer == NULL) {
		fclose(file);
		return NULL;
	}

	memcpy(buffer, header, header_len);
	size_t bytes_read = fread(buffer + header_len, 1, file_size, file);
	fclose(file);

	if ((long)bytes_read != file_size) {
		free(buffer);
		return not_found(size);
	}

	*size = header_len + file_size;
	return buffer;
}
