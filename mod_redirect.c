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
#include "apr_strings.h"

/* The Framework and the header */
module AP_MODULE_DECLARE_DATA redirect_module;

static const char hook[] = "redirect";
static const char image0[] = ".jpg";
static const char image1[] = ".png";
static const char image2[] = ".bmp";
static const char image3[] = ".gif";
static const char image4[] = ".raw";
static const char method[] = "GET";

typedef struct redirect_cfg{
    int redirect;
}redirect_cfg;
/* Define the user function */


/* New str copy fucntion */
char *strcpy_head( char *src, char *ingredients,request_rec *r){
    if(ingredients == NULL){
        return src;
    }
    char *mid = strstr(src,ingredients);
    size_t size = (mid - src);
    char *newstring =apr_palloc(r->pool,sizeof(char)*(mid-src)+1);
    strncpy(newstring,src, size);
    return newstring;
}

char* strcpy_tail( char *src, char *ingredients, request_rec *r){
    if(ingredients == NULL){
        return src;
    }
    char *mid = strstr(src,ingredients);
    size_t size0 = strlen(ingredients);
    mid = mid + size0;
    size_t size1 = strlen(src) - (mid-src);
    char *newstring = apr_palloc(r->pool,sizeof(char)*(size1+1));
    strncpy(newstring,mid,size1);
    return newstring;
}

/* main function   */
static int redirect_handler(request_rec *r)
{
    redirect_cfg *cfg =
                        ap_get_module_config(r->per_dir_config, &redirect_module) ;
    //if(!cfg->redirect){
      //  return DECLINED ;
    //}
    /*
    if ( r->method_number != M_GET ) {
        return DECLINED ;
    }
    
    if (!r->uri){
        return DECLINED;
    }
     */
    char *uri = r->uri;
    const char *Referer = apr_table_get(r->headers_in,"Referer");
    char *ip = r->useragent_ip;
    // char *ingredients = apr_pstrcat(r->pool, ip,Referer,NULL);
    char ingredients []= "10.211.55.2johnnytest.jpg";
    /* The request have to get an image */
    
    if(strstr(uri,ingredients)){
        char *head = strcpy_head(uri,ingredients,r);
        char *tail = strcpy_tail(uri,ingredients,r);
        char *newurl = apr_pstrcat(r->pool,head,tail,NULL);
        //ap_internal_redirect(newurl, r);
        r->uri = newurl;
        return APR_SUCCESS;
    }
        
    
    /*
    if(strstr(uri,"test")){
        char *head = strcpy_head(uri,NULL,r);
        char *tail = strcpy_tail(uri,NULL,r);
        const char *newurl = apr_pstrcat(r->pool,head,tail,NULL);
        //ap_internal_redirect(newurl, r);
        r->uri = newurl;
        return APR_SUCCESS;
    }
     */
    
    return OK;
}

static void register_hooks(apr_pool_t *pool){
    ap_hook_header_parser(redirect_handler,NULL,NULL,APR_HOOK_FIRST);
}
static void* redirect_cr_cfg(apr_pool_t* pool, char* x) {
    redirect_cfg *ret = apr_pcalloc(pool, sizeof(redirect_cfg));
    return ret ;
}
/*
static const command_rec redirect_cmds[] = {
  AP_INIT_FLAG("redirect", ap_set_flag_slot,
    (void*)APR_OFFSETOF(redirect_cfg, redirect), ACCESS_CONF,
    "Enable redirection"),
    {NULL} };
*/
/*
static const char *redirectEnable(cmd_parms *cmd, void *dummy, int arg)
{
    redirect_cfg *pConfig= ap_get_module_config(cmd->server->module_config,&redirect_module);
    pConfig->redirect= 1;
    return NULL;
}
 */

static const command_rec redirectcmds[] =
{
    AP_INIT_FLAG("redirect",ap_set_flag_slot, (void*)APR_OFFSETOF(redirect_cfg, redirect), ACCESS_CONF,
                 "Run a rewriter filter on this host"),
    {NULL}
};


/* Define module as an entity and assign a function for registering hooks*/
module AP_MODULE_DECLARE_DATA redirect_module =
{
    STANDARD20_MODULE_STUFF,
    redirect_cr_cfg,
    NULL,
    NULL,
    NULL,
    redirectcmds,
    register_hooks   /* Our hook registering function */
};





/*
static void register_hooks(apr_pool_t *pool);
static int project_handler(request_rec *r);


/* Register hooks: Adds a hook to httpd process */
/*
static void register_hooks(apr_pool_t *pool)
{
    /* Create a hook in the request handler, so we get called when a request arrives*/
/*
    ap_hook_handler(project_handler,NULL,NULL,APR_HOOK_FIRST);
}



/* handler  */
/*
static int project_handler(request_rec *r)
{
    /* First of all, we need to check if this is a call for the */
    /* If it is, we accpet it and do our things, if not, we simply return DECLINED */
    /* and server will try somewhere else */
    /*
    if(!r->handler || strcmp(r->handler,"project_handler")) return (DECLINED);
    
    //printf("Work in the project-handler funtion \n");
    /* Declare the variables we need for our program */
    /*
    const char *methodType;
    char *uri;
    const char *Referer;
    char *ip;
    char *ingredients;
    
    /* Get Needed Value from the header_in table */
/*
    Referer = apr_table_get(r->headers_in,"Referer");
    /* Print out the referer */
    //printf("The referer %s \n",Referer);
    /*
    methodType = r->method;

    /* Print out the method*/
    //printf("The  method %s \n",methodType);
    
    /* The IP address */
/*
    ip = r->useragent_ip;
    //printf("The  ip %s \n",ip);
    
    /* The uri*/
/*
    uri = r->uri;
    //printf("The  uri %s \n",ip);
    
    /* Th ingredients*/
/*
    ingredients = apr_pstrcat(r->pool, ip,Referer,NULL);
    //printf("The  ingredietns %s \n",ingredients);

    /* Find out the situation we need */
    /*
    if(!strcmp(methodType,"GET")){
        return OK;
    }
    
    /* The request have to get an image */
/*
    if(strstr(uri,ingredients)){
        char *head = strcpy_head(uri,ingredients,r);
        char *tail = strcpy_tail(uri,ingredients,r);
        char *newurl = apr_pstrcat(r->pool,head,tail,NULL);
        
        r->uri = newurl;
        
        return OK;
    }else{
       
        return OK;
    }
    
    return OK;
}
*/

    

