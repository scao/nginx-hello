#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static char *ngx_http_hello(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_command_t ngx_http_hello_commands[] = {
  {
    ngx_string("hello"),
    NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
    ngx_http_hello,
    0,
    0,
    NULL
  },

  ngx_null_command
};

static u_char hello[] = "hello!";

static ngx_http_module_t ngx_http_hello_module_ctx = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

ngx_module_t ngx_http_hello_module = {
  NGX_MODULE_V1,
  &ngx_http_hello_module_ctx,
  &ngx_http_hello_commands[0],
  NGX_HTTP_MODULE,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NGX_MODULE_V1_PADDING
};

static ngx_int_t ngx_http_hello_handler(ngx_http_request_t *r)
{
  ngx_buf_t *b;
  ngx_chain_t out;

  r->headers_out.content_type.len = sizeof("text/plain") -1;
  r->headers_out.content_type.data = (u_char *)"text/plain";

  b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

  out.buf = b;
  out.next = NULL;

  b->pos = hello;
  b->last = hello + sizeof(hello) -1;
  b->memory = 1;
  b->last_buf = 1;

  r->headers_out.status = NGX_HTTP_OK;
  r->headers_out.content_length_n = sizeof(hello) -1;
  ngx_http_send_header(r);

  return ngx_http_output_filter(r, &out);
}

static char *ngx_http_hello(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
  ngx_http_core_loc_conf_t *clcf;
  clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
  clcf->handler = ngx_http_hello_handler;

  return NGX_CONF_OK;
}
