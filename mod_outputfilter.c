#include "util_filter.h"
#include "apr_lib.h"
#include "apr_general.h"
#include "apr_buckets.h"
#include <ctype.h>
#include <stdio.h>
#include "apr_hash.h"
#include "ap_config.h"
#include "ap_provider.h"
#include "httpd.h"
#include "http_core.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_request.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "apr_strings.h"

static const char filter[] = "rewritefilter";
/* We can extend as much as imgae type as we want */
static const char match0[] = ".jpg";
static const char match1[] = ".png";
static const char match2[] = ".bmp";
static const char match3[] = ".gif";
static const char match4[] = ".raw";

module AP_MODULE_DECLARE_DATA rewriter_filter_module;

typedef struct
{
        int bEnabled;

}rewritefilter;

static void *rewriterFilterCreateServerConfig(apr_pool_t *p, server_rec *s)
{
    rewritefilter *pConfig = apr_pcalloc(p,sizeof *pConfig);
    pConfig->bEnabled;
    return pConfig;
}

static void rewriterFilterInsertFilter(request_rec *r)
{
    rewritefilter *pConfig=ap_get_module_config(r->server->module_config,&rewriter_filter_module);
    
    if(!(pConfig->bEnabled))return;

    ap_add_output_filter(filter, NULL,r,r->connection);
    
}

char *join(char *a, const char *b,request_rec *r) {
    
    if(a ==NULL || b ==NULL){
        return NULL;
    }
    char *c = apr_palloc(r->pool,sizeof(char)*(strlen(a) + strlen(b))+1);
    char *temp = c;
    if(c==NULL){
        return NULL;
    }
    if (c == NULL) exit (1);
    for(int j = 0; j < strlen(a);j++){
        *c++ = *(a+j);
    }
    for(int m = 0 ; m < strlen(b);m++){
        *c++ = *(b+m);
    }
     *(c + sizeof(char)*(strlen(a) + strlen(b)+1 )) = '\0';
    return temp;
}

static const char *appendFunc( const char *src, const char match[], char ingredients[],request_rec *r)
{
    /* Define the pointers */
    const char *srcp = src;
    char *ingredientsp = ingredients;
    
    if(srcp == NULL){
        return NULL;
    }
    if(match == NULL){
        return NULL;
    }
    
    if(ingredientsp == NULL){
        return NULL;
    }
    
    char *mid = strstr(src,match);
   
    if(mid == NULL){
        return src;
    }
    char *temarray =apr_palloc(r->pool,sizeof(char)*(mid -srcp)+strlen(ingredientsp)+strlen(match)+1);
    *( temarray + (mid-srcp) + strlen(ingredientsp)+strlen(match)+1)='\0';
    for(int j = 0; j < mid-src; j++){
        *(temarray+j) = *(srcp+j);
    }
    
    temarray = apr_pstrcat(r->pool,temarray,ingredientsp,NULL);
    temarray = apr_pstrcat(r->pool,temarray,match,NULL);
    mid = strstr(src,match);
    if(*(mid+4) == '\0'){
        const char*temarray1 = temarray;
        return temarray1;
    }else{
        mid++;
        mid++;
        mid++;
        mid++;
        src = join(temarray,appendFunc(mid,match,ingredientsp,r),r);
    }
    return src;
}

static apr_status_t rewriterfilterfunction(ap_filter_t *f,apr_bucket_brigade *pbbIn)
{
    /* Framework */
    request_rec *r = f->r;
    conn_rec *c = r->connection;
    apr_bucket *pbkIn;
    apr_bucket_brigade *pbbOut;
    const char *Refferer;
    char *ingredients;
    char *ip;
    pbbOut = apr_brigade_create(r->pool, c->bucket_alloc);
    /* Framework*/
     
    /* Main logic*/
    ip = r->useragent_ip;
    Refferer = apr_table_get(r->headers_in,"Referer");
    ingredients = apr_pstrcat(r->pool, ip,Refferer,NULL);
    /* Main logic */
    
    /* Framework*/
    for(pbkIn = APR_BRIGADE_FIRST(pbbIn);
        pbkIn != APR_BRIGADE_SENTINEL(pbbIn);
        pbkIn = APR_BUCKET_NEXT(pbkIn))
    {
    
        const char *data;
        
        apr_size_t len;
        
        char *buf;
        
        apr_size_t n;
        
        apr_bucket *pbktOut;
    /* Framework */

    /* Main Logic */
        apr_size_t newsize;
    /* Main Logic */

    /* Framework */
        if(APR_BUCKET_IS_EOS(pbkIn))
        {
            apr_bucket *pbktEOS=apr_bucket_eos_create(c->bucket_alloc);
            APR_BRIGADE_INSERT_TAIL(pbbOut,pbktEOS);
            continue;
        }
        apr_bucket_read(pbkIn, &data, &len, APR_BLOCK_READ);
        buf = apr_bucket_alloc(len, c->bucket_alloc);
        for (n=0 ; n < len ; ++n)buf[n] = data[n];
    /* Framework */
    
    /* Main Logic */
        const char *result;
        
        if(strstr(buf,match0) || strstr(buf,match1) || strstr(buf,match2) || strstr(buf,match3) ||strstr(buf,match4)){
            if(strstr(buf,match0)){
                result = appendFunc(buf,match0,ingredients,r);
            }
            
            if(strstr(result,match1)){
                result = appendFunc(result,match1,ingredients,r);
            }
                  
                       
            if(strstr(result,match2)){
                result = appendFunc(result,match2,ingredients,r);
            }
            
            if(strstr(result,match3)){
                result = appendFunc(result,match3,ingredients,r);
            }
                  
            if(strstr(result,match4)){
                result = appendFunc(result,match4,ingredients,r);
            }
        }
        newsize = strlen(result);
    /* Main Logic */
        
    /* Framework */
        pbktOut = apr_bucket_heap_create(result, newsize, apr_bucket_free,c->bucket_alloc);
        APR_BRIGADE_INSERT_TAIL(pbbOut,pbktOut);
    }
    apr_brigade_cleanup(pbbIn);
    return ap_pass_brigade(f->next,pbbOut);
    /* Framework */
               
}

static const char *rewriterFilterEnable(cmd_parms *cmd, void *dummy, int arg)
{
    rewritefilter *pConfig= ap_get_module_config(cmd->server->module_config,&rewriter_filter_module);
    pConfig->bEnabled= 1;
    
    return NULL;
}

static const command_rec rewriterFilterCmd[] =
{
    AP_INIT_FLAG("rewritefilter",rewriterFilterEnable, NULL, RSRC_CONF,
                 "Run a rewriter filter on this host"),
    {NULL}
};

static void rewriterFilterRegisterHooks(apr_pool_t *p)
{
    ap_hook_insert_filter(rewriterFilterInsertFilter, NULL, NULL, APR_HOOK_LAST);
    ap_register_output_filter(filter,rewriterfilterfunction,NULL,
                              AP_FTYPE_RESOURCE);
}

module AP_MODULE_DECLARE_DATA rewriter_filter_module =
{
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    rewriterFilterCreateServerConfig,
    NULL,
    rewriterFilterCmd,
    rewriterFilterRegisterHooks
};
