#include <jni.h>
#include <bdd.h>
#include <fdd.h>
#include <stdlib.h>
#include <time.h>
#include "buddy_jni.h"

/*
** When casting from `int' to a pointer type, you should
** first cast to `intptr_cast_type'.  This is a type
** that is (a) the same size as a pointer, on most platforms,
** to avoid compiler warnings about casts from pointer to int of
** different size; and (b) guaranteed to be at least as big as
** `int'.
*/
#if __STDC_VERSION__ >= 199901
  #include <inttypes.h>
  #if INTPTR_MAX >= INT_MAX
    typedef intptr_t intptr_cast_type;
  #else /* no intptr_t, or intptr_t smaller than `int' */
    typedef intmax_t intptr_cast_type;
  #endif
#else
  #include <stddef.h>
  #include <limits.h>
  #if PTRDIFF_MAX >= INT_MAX
    typedef ptrdiff_t intptr_cast_type;
  #else
    typedef int intptr_cast_type;
  #endif
#endif

#define INVALID_BDD -1

static int bdd_error;

static void bdd_errhandler(int errcode)
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_errstring(%d)\n", errcode);
#endif
  //printf("BuDDy error: %s\n", bdd_errstring(errcode));
  bdd_error = errcode;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_clear_error()\n");
#endif
  bdd_clear_error();
}

static int check_error()
{
  int err = bdd_error;
  if (!err) return 0; // fast path
  switch (err) {
  case BDD_MEMORY:   /* Out of memory */
    throw_new_OutOfMemoryError( bdd_errstring(err) );
    break;
  case BDD_VAR:      /* Unknown variable */
  case BDD_RANGE:    /* Variable value out of range (not in domain) */
  case BDD_DEREF:    /* Removing external reference to unknown node */
  case BDD_RUNNING:  /* Called bdd_init() twice whithout bdd_done() */
  case BDD_ORDER:    /* Vars. not in order for vector based functions */
  case BDD_BREAK:    /* User called break */
  case BDD_VARNUM:  /* Different number of vars. for vector pair */
  case BDD_OP:      /* Unknown operator */
  case BDD_VARSET:  /* Illegal variable set */
  case BDD_VARBLK:  /* Bad variable block operation */
  case BDD_DECVNUM: /* Trying to decrease the number of variables */
  case BDD_REPLACE: /* Replacing to already existing variables */
  case BDD_NODENUM: /* Number of nodes reached user defined maximum */
  case BVEC_SIZE:    /* Mismatch in bitvector size */
  case BVEC_DIVZERO: /* Division by zero */
    throw_new_BDDException( bdd_errstring(err) );
    break;
  case BDD_FILE:     /* Some file operation failed */
  case BDD_FORMAT:   /* Incorrect file format */
    throw_new_IOException( bdd_errstring(err) );
    break;
  case BDD_NODES:   /* Tried to set max. number of nodes to be fewer */
                    /* than there already has been allocated */
  case BDD_ILLBDD:  /* Illegal bdd argument */
  case BDD_SIZE:    /* Illegal size argument */
  case BVEC_SHIFT:   /* Illegal shift-left/right parameter */
    throw_new_BDDException( bdd_errstring(err) );
    break;
  default:
    throw_new_InternalError( bdd_errstring(err) );
    break;
  }
  return err;
}

static void bdd_gbchandler(int code, bddGbcStat *s)
{
  BuDDyFactory factory_obj;
  GCStats gc_obj;

  factory_obj = get_BuDDyFactory_INSTANCE();
  if (!factory_obj) {
    printf("Error: BuDDyFactory.INSTANCE is null\n");
    return;
  }

  gc_obj = get_BDDFactory_gcstats( (BDDFactory)factory_obj ); // XY TODO real cast
  if (!gc_obj) {
    printf("Error: gcstats is null\n");
    return;
  }

  set_GCStats_nodes( gc_obj, s->nodes );
  //if ( staticjni_errno ) {
    //printf("Error: setting nodes failed\n");
	//return;
  //}

  set_GCStats_freenodes( gc_obj, s->freenodes );

  long t = s->time;
  if (CLOCKS_PER_SEC < 1000) {
    t = t * 1000 / CLOCKS_PER_SEC;
  }
  else {
    t /= (CLOCKS_PER_SEC/1000);
  }
  set_GCStats_time( gc_obj, t );

  t = s->sumtime;
  if (CLOCKS_PER_SEC < 1000) {
    t = t * 1000 / CLOCKS_PER_SEC;
  }
  else {
    t /= (CLOCKS_PER_SEC/1000);
  }
  set_GCStats_sumtime( gc_obj, t );

  set_GCStats_num( gc_obj, t );

  BuDDyFactory_gc_callback( code );
}

/**** START OF NATIVE METHOD IMPLEMENTATIONS ****/

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    registerNatives
 * Signature: ()V
 */
void BuDDyFactory_registerNatives__impl(){ }

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    buildCube0
 * Signature: (I[I)I
 */
jint BuDDyFactory_buildCube0__impl
  ( jint value, jintArray arr )
{
  jint width, r;
  jint* a;

  access_jintArray( arr, a, width ) {
#if defined(TRACE_BUDDYLIB)
    printf("bdd_buildcube(%d, %d, %p)\n", value, width, a);
#endif
    r = bdd_buildcube(value, width, (int*)a);
  }
  check_error(); // XY
  return r;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    buildCube1
 * Signature: (I[I)I
 */
jint BuDDyFactory_buildCube1__impl
  ( jint value, jintArray arr )
{
  jint width, r;
  jint* a;

  access_jintArray( arr, a, width ) {
#if defined(TRACE_BUDDYLIB)
    printf("bdd_ibuildcube(%d, %d, %p)\n", value, width, a);
#endif
    r = bdd_ibuildcube(value, width, (int*)a);
  }
  check_error();
  return r;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    makeSet0
 * Signature: ([I)I
 */
jint BuDDyFactory_makeSet0__impl
  ( jintArray arr )
{
  jint width, r;
  jint* a;

  access_jintArray( arr, a, width ) {
#if defined(TRACE_BUDDYLIB)
    printf("bdd_makeset(%p, %d)\n", a, width);
#endif
    r = bdd_makeset((int*)a, width);
  }
  check_error();
  return r;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    initialize0
 * Signature: (II)V
 */
//JNIEXPORT void JNICALL Java_net_sf_javabdd_BuDDyFactory_initialize0
//  (JNIEnv *thread_env, jobject o, jint nodesize, jint cachesize) // XY type error, is static, not a method
void BuDDyFactory_initialize0__impl
  ( jint nodesize, jint cachesize )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_init(%d, %d)\n", nodesize, cachesize);
#endif
  bdd_init(nodesize, cachesize);
#if defined(TRACE_BUDDYLIB)
  printf("bdd_error_hook(%p)\n", bdd_errhandler);
#endif
  bdd_error_hook(bdd_errhandler);
#if defined(TRACE_BUDDYLIB)
  printf("bdd_resize_hook(%p)\n", BuDDyFactory_resize_callback);
#endif
  bdd_resize_hook(BuDDyFactory_resize_callback);
#if defined(TRACE_BUDDYLIB)
  printf("bdd_gbc_hook(%p)\n", bdd_gbchandler);
#endif
  bdd_gbc_hook(bdd_gbchandler);
#if defined(TRACE_BUDDYLIB)
  printf("bdd_reorder_hook(%p)\n", BuDDyFactory_reorder_callback);
#endif
  bdd_reorder_hook(BuDDyFactory_reorder_callback);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    isInitialized0
 * Signature: ()Z
 */
jboolean BuDDyFactory_isInitialized0__impl( )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_isrunning()\n");
#endif
  return bdd_isrunning();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    done0
 * Signature: ()V
 */
void BuDDyFactory_done0__impl( )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_done()\n");
#endif
  bdd_done();
  check_error();
}

extern int bdderrorcond;

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    setError0
 * Signature: (I)V
 */
void BuDDyFactory_setError0__impl
  ( jint code )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_setError(%d)\n", code);
#endif
  bdderrorcond = code;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    clearError0
 * Signature: ()V
 */
void BuDDyFactory_clearError0__impl
  (  )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_clearError()\n");
#endif
  bdderrorcond = 0;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    setMaxNodeNum0
 * Signature: (I)I
 */
jint BuDDyFactory_setMaxNodeNum0__impl
  ( jint size )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_setmaxnodenum(%d)\n", size);
#endif
  result = bdd_setmaxnodenum(size);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    setNodeTableSize0
 * Signature: (I)I
 */
jint BuDDyFactory_setNodeTableSize0__impl
  ( jint size )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_setallocnum(%d)\n", size);
#endif
  result = bdd_setallocnum(size);
  check_error();
  return result;
}

/* XY error not implemented

jint BuDDyFactory_setCacheSize0__impl
  ( jint size ) {
}
*/

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    setMinFreeNodes0
 * Signature: (I)I
 */
jint BuDDyFactory_setMinFreeNodes0__impl
  ( jint n )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_setminfreenodes(%d)\n", n);
#endif
  result = bdd_setminfreenodes(n);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    setMaxIncrease0
 * Signature: (I)I
 */
jint BuDDyFactory_setMaxIncrease0__impl
  ( jint size )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_setmaxincrease(%d)\n", size);
#endif
  result = bdd_setmaxincrease(size);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    setIncreaseFactor0
 * Signature: (D)D
 */
jdouble BuDDyFactory_setIncreaseFactor0__impl
  ( jdouble r )
{
  jdouble result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_setincreasefactor(%lf)\n", r);
#endif
  result = bdd_setincreasefactor(r);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    setCacheRatio0
 * Signature: (I)I
 */
jint BuDDyFactory_setCacheRatio0__impl
  ( jint r )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_setcacheratio(%d)\n", r);
#endif
  result = bdd_setcacheratio(r);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    varNum0
 * Signature: ()I
 */
jint BuDDyFactory_varNum0__impl
  (  )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_varnum()\n");
#endif
  result = bdd_varnum();
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    setVarNum0
 * Signature: (I)I
 */
jint BuDDyFactory_setVarNum0__impl
  ( jint num )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_setvarnum(%d)\n", num);
#endif
  result = bdd_setvarnum(num);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    duplicateVar0
 * Signature: (I)I
 */
jint BuDDyFactory_duplicateVar0__impl
  ( jint var )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_duplicatevar(%d)\n", var);
#endif
  result = bdd_duplicatevar(var);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    extVarNum0
 * Signature: (I)I
 */
jint BuDDyFactory_extVarNum0__impl
  ( jint num )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_extvarnum(%d)\n", num);
#endif
  result = bdd_extvarnum(num);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    ithVar0
 * Signature: (I)I
 */
jint BuDDyFactory_ithVar0__impl
  ( jint var )
{
  BDD b;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_ithvar(%d)\n", var);
#endif
  b = bdd_ithvar(var);
  check_error();
  return b;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    nithVar0
 * Signature: (I)I
 */
jint BuDDyFactory_nithVar0__impl
  ( jint var )
{
  BDD b;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_nithvar(%d)\n", var);
#endif
  b = bdd_nithvar(var);
  check_error();
  return b;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    swapVar0
 * Signature: (II)V
 */
void BuDDyFactory_swapVar0__impl
  ( jint v1, jint v2 )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_swapvar(%d, %d)\n", v1, v2);
#endif
  bdd_swapvar(v1, v2);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    makePair0
 * Signature: ()J
 */
jlong BuDDyFactory_makePair0__impl
  (  )
{
  bddPair* pair;
  jlong r;

#if defined(TRACE_BUDDYLIB)
  printf("bdd_newpair()\n");
#endif
  pair = bdd_newpair();
  r = (jlong) (intptr_cast_type) pair;
  return r;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    printAll0
 * Signature: ()V
 */
void BuDDyFactory_printAll0__impl
  (  )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_printall()\n");
#endif
  bdd_printall();
  fflush(stdout);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    printTable0
 * Signature: (I)V
 */
void BuDDyFactory_printTable0__impl
  ( jint bdd )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_printtable(%d)\n", bdd);
#endif
  bdd_printtable(bdd);
  fflush(stdout);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    load0
 * Signature: (Ljava/lang/String;)I
 */
jint BuDDyFactory_load0__impl
  ( jstring fname )
{
  BDD r;
  int rc;
  char *str;

  str = (char*) (*thread_env)->GetStringUTFChars(thread_env, fname, NULL);
  if (str == NULL) return -1;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_fnload(%s, %p)\n", str, &r);
#endif
  rc = bdd_fnload(str, &r);
  (*thread_env)->ReleaseStringUTFChars(thread_env, fname, str);
  check_error();
  return r;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    save0
 * Signature: (Ljava/lang/String;I)V
 */
void BuDDyFactory_save0__impl
  ( jstring fname, jint r )
{
  int rc;
  char *str;

  str = (char*) (*thread_env)->GetStringUTFChars(thread_env, fname, NULL);
  if (str == NULL) return;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_fnsave(%s, %d)\n", str, r);
#endif
  rc = bdd_fnsave(str, r);
  (*thread_env)->ReleaseStringUTFChars(thread_env, fname, str);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    level2Var0
 * Signature: (I)I
 */
jint BuDDyFactory_level2Var0__impl
  ( jint level )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_level2var(%d)\n", level);
#endif
  result = bdd_level2var(level);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    var2Level0
 * Signature: (I)I
 */
jint BuDDyFactory_var2Level0__impl
  ( jint var )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_var2level(%d)\n", var);
#endif
  result = bdd_var2level(var);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    reorder0
 * Signature: (I)V
 */
void BuDDyFactory_reorder0__impl
  ( jint method )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_reorder(%d)\n", method);
#endif
  bdd_reorder(method);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    autoReorder0
 * Signature: (I)V
 */
void BuDDyFactory_autoReorder0__impl
  ( jint method )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_autoreorder(%d)\n", method);
#endif
  bdd_autoreorder(method);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    autoReorder1
 * Signature: (II)V
 */
void BuDDyFactory_autoReorder1__impl
  ( jint method, jint n )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_autoreorder_times(%d, %d)\n", method, n);
#endif
  bdd_autoreorder_times(method, n);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    getReorderMethod0
 * Signature: ()I
 */
jint BuDDyFactory_getReorderMethod0__impl
  (  )
{
  int method;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_getreorder_method()\n");
#endif
  method = bdd_getreorder_method();
  check_error();
  return method;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    getReorderTimes0
 * Signature: ()I
 */
jint BuDDyFactory_getReorderTimes0__impl
  (  )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_getreorder_times()\n");
#endif
  result = bdd_getreorder_times();
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    disableReorder0
 * Signature: ()V
 */
void BuDDyFactory_disableReorder0__impl
  (  )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_disable_reorder()\n");
#endif
  bdd_disable_reorder();
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    enableReorder0
 * Signature: ()V
 */
void BuDDyFactory_enableReorder0__impl
  (  )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_enable_reorder()\n");
#endif
  bdd_enable_reorder();
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    reorderVerbose0
 * Signature: (I)I
 */
jint BuDDyFactory_reorderVerbose0__impl
  ( jint level )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_reorder_verbose(%d)\n", level);
#endif
  result = bdd_reorder_verbose(level);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    setVarOrder0
 * Signature: ([I)V
 */
void BuDDyFactory_setVarOrder0__impl
  ( jintArray arr )
{
  jint *a;
  jint size, varnum;

  size = get_length_jintArray(arr);
  varnum = bdd_varnum();
  if (size != varnum) {
    throw_new_IllegalArgumentException("array size != number of vars");
    return;
  }
  a = get_access_jintArray(arr);
#if defined(TRACE_BUDDYLIB)
  printf("bdd_setvarorder(%p)\n", a);
#endif
  bdd_setvarorder((int*)a);
  release_access_jintArray(arr, a);

  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    addVarBlock0
 * Signature: (IZ)V
 */
void BuDDyFactory_addVarBlock0__impl
  ( jint var, jboolean fixed )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_addvarblock(%d, %d)\n", var , fixed);
#endif
  bdd_addvarblock(var, fixed);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    addVarBlock1
 * Signature: (IIZ)V
 */
void BuDDyFactory_addVarBlock1__impl
  ( jint first, jint last, jboolean fixed )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_intaddvarblock(%d, %d, %d)\n", first, last, fixed);
#endif
  bdd_intaddvarblock(first, last, fixed);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    varBlockAll0
 * Signature: ()V
 */
void BuDDyFactory_varBlockAll0__impl
  (  )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_varblockall()\n");
#endif
  bdd_varblockall();
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    clearVarBlocks0
 * Signature: ()V
 */
void BuDDyFactory_clearVarBlocks0__impl
  (  )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_clrvarblocks()\n");
#endif
  bdd_clrvarblocks();
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    printOrder0
 * Signature: ()V
 */
void BuDDyFactory_printOrder0__impl
  (  )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_printorder()\n");
#endif
  bdd_printorder();
  fflush(stdout);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    nodeCount0
 * Signature: ([I)I
 */
jint BuDDyFactory_nodeCount0__impl
  ( jintArray arr )
{
  jint *a;
  jint size;
  int result;

  access_jintArray( arr, a, size ) {
#if defined(TRACE_BUDDYLIB)
    printf("bdd_anodecount(%p, %d)\n", a, size);
#endif
    result = bdd_anodecount((int*)a, size);
  }
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    getAllocNum0
 * Signature: ()I
 */
jint BuDDyFactory_getAllocNum0__impl
  (  )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_getallocnum()\n");
#endif
  result = bdd_getallocnum();
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    getCacheSize0
 * Signature: ()I
 */
jint BuDDyFactory_getCacheSize0__impl
  (  )
{
  int result;
  bddStat stats;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_stats(%p)\n", &stats);
#endif
  bdd_stats(&stats);
  result = stats.cachesize;
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    getNodeNum0
 * Signature: ()I
 */
jint BuDDyFactory_getNodeNum0__impl
  (  )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_getnodenum()\n");
#endif
  result = bdd_getnodenum();
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    reorderGain0
 * Signature: ()I
 */
jint BuDDyFactory_reorderGain0__impl
  (  )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_reorder_gain()\n");
#endif
  result = bdd_reorder_gain();
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    printStat0
 * Signature: ()V
 */
void BuDDyFactory_printStat0__impl
  (  )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_printstat()\n");
#endif
  bdd_printstat();
  fflush(stdout);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory
 * Method:    getVersion0
 * Signature: ()Ljava/lang/String;
 */
jstring BuDDyFactory_getVersion0__impl
  (  )
{
  char *buf;
  jstring result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_versionstr()\n");
#endif
  buf = bdd_versionstr();
  result = (*thread_env)->NewStringUTF(thread_env, buf);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    var0
 * Signature: (I)I
 */
jint BuDDyBDD_var0__impl
  ( jint b )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_var(%d)\n", b);
#endif
  result = bdd_var(b);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    high0
 * Signature: (I)I
 */
jint BuDDyBDD_high0__impl
  ( jint b )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_high(%d)\n", b);
#endif
  result = bdd_high(b);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    low0
 * Signature: (I)I
 */
jint BuDDyBDD_low0__impl
  ( jint b )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_low(%d)\n", b);
#endif
  result = bdd_low(b);
  check_error();
  return result;}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    not0
 * Signature: (I)I
 */
jint BuDDyBDD_not0__impl
  ( jint b )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_not(%d)\n", b);
#endif
  result = bdd_not(b);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    ite0
 * Signature: (III)I
 */
jint BuDDyBDD_ite0__impl
  ( jint b, jint c, jint d )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_ite(%d, %d, %d)\n", b, c, d);
#endif
  result = bdd_ite(b, c, d);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    relprod0
 * Signature: (III)I
 */
jint BuDDyBDD_relprod0__impl
  ( jint b, jint c, jint d )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_relprod(%d, %d, %d)\n", b, c, d);
#endif
  result = bdd_relprod(b, c, d);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    compose0
 * Signature: (III)I
 */
jint BuDDyBDD_compose0__impl
  ( jint b, jint c, jint v )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_compose(%d, %d, %d)\n", b, c, v);
#endif
  result = bdd_compose(b, c, v);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    constrain0
 * Signature: (II)I
 */
jint BuDDyBDD_constrain0__impl
  ( jint b, jint c )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_constrain(%d, %d)\n", b, c);
#endif
  result = bdd_constrain(b, c);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    exist0
 * Signature: (II)I
 */
jint BuDDyBDD_exist0__impl
  ( jint b, jint c )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_exist(%d, %d)\n", b, c);
#endif
  result = bdd_exist(b, c);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    forAll0
 * Signature: (II)I
 */
jint BuDDyBDD_forAll0__impl
  ( jint b, jint c )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_forall(%d, %d)\n", b, c);
#endif
  result = bdd_forall(b, c);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    unique0
 * Signature: (II)I
 */
jint BuDDyBDD_unique0__impl
  ( jint b, jint c )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_unique(%d, %d)\n", b, c);
#endif
  result = bdd_unique(b, c);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    restrict0
 * Signature: (II)I
 */
jint BuDDyBDD_restrict0__impl
  ( jint b, jint c )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_restrict(%d, %d)\n", b, c);
#endif
  result = bdd_restrict(b, c);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    simplify0
 * Signature: (II)I
 */
jint BuDDyBDD_simplify0__impl
  ( jint b, jint c )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_simplify(%d, %d)\n", b, c);
#endif
  result = bdd_simplify(b, c);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    support0
 * Signature: (I)I
 */
jint BuDDyBDD_support0__impl
  ( jint b )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_support(%d)\n", b);
#endif
  result = bdd_support(b);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    apply0
 * Signature: (III)I
 */
jint BuDDyBDD_apply0__impl
  ( jint b, jint c, jint operation )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_apply(%d, %d, %d)\n", b, c, operation);
#endif
  result = bdd_apply(b, c, operation);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    applyAll0
 * Signature: (IIII)I
 */
jint BuDDyBDD_applyAll0__impl
  ( jint b, jint c, jint operation, jint d )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_appall(%d, %d, %d, %d)\n", b, c, operation, d);
#endif
  result = bdd_appall(b, c, operation, d);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    applyEx0
 * Signature: (IIII)I
 */
jint BuDDyBDD_applyEx0__impl
  ( jint b, jint c, jint operation, jint d )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_appex(%d, %d, %d, %d)\n", b, c, operation, d);
#endif
  result = bdd_appex(b, c, operation, d);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    applyUni0
 * Signature: (IIII)I
 */
jint BuDDyBDD_applyUni0__impl
  ( jint b, jint c, jint operation, jint d )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_appuni(%d, %d, %d, %d)\n", b, c, operation, d);
#endif
  result = bdd_appuni(b, c, operation, d);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    satOne0
 * Signature: (I)I
 */
jint BuDDyBDD_satOne0__impl
  ( jint b )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_satone(%d)\n", b);
#endif
  result = bdd_satone(b);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    fullSatOne0
 * Signature: (I)I
 */
jint BuDDyBDD_fullSatOne0__impl
  ( jint b )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_fullsatone(%d)\n", b);
#endif
  result = bdd_fullsatone(b);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    satOne1
 * Signature: (III)I
 */
jint BuDDyBDD_satOne1__impl
  ( jint b, jint c, jint d )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_satoneset(%d, %d, %d)\n", b, c, d);
#endif
  result = bdd_satoneset(b, c, d);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    printSet0
 * Signature: (I)V
 */
void BuDDyBDD_printSet0__impl
  ( jint b )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_printset(%d)\n", b);
#endif
  bdd_printset(b);
  fflush(stdout);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    printDot0
 * Signature: (I)V
 */
void BuDDyBDD_printDot0__impl
  ( jint b )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_printdot(%d)\n", b);
#endif
  bdd_printdot(b);
  fflush(stdout);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    nodeCount0
 * Signature: (I)I
 */
jint BuDDyBDD_nodeCount0__impl
  ( jint b )
{
  int result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_nodecount(%d)\n", b);
#endif
  result = bdd_nodecount(b);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    pathCount0
 * Signature: (I)D
 */
jdouble BuDDyBDD_pathCount0__impl
  ( jint b )
{
  double result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_pathcount(%d)\n", b);
#endif
  result = bdd_pathcount(b);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    satCount0
 * Signature: (I)D
 */
jdouble BuDDyBDD_satCount0__impl
  ( jint b )
{
  double result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_satcount(%d)\n", b);
#endif
  result = bdd_satcount(b);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    satCount1
 * Signature: (II)D
 */
jdouble BuDDyBDD_satCount1__impl
  ( jint b, jint c )
{
  double result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_satcountset(%d, %d)\n", b, c);
#endif
  result = bdd_satcountset(b, c);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    logSatCount0
 * Signature: (I)D
 */
jdouble BuDDyBDD_logSatCount0__impl
  ( jint b )
{
  double result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_satcountln(%d)\n", b);
#endif
  result = bdd_satcountln(b);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    logSatCount1
 * Signature: (II)D
 */
jdouble BuDDyBDD_logSatCount1__impl
  ( jint b, jint c )
{
  double result;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_satcountlnset(%d, %d)\n", b, c);
#endif
  result = bdd_satcountlnset(b, c);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    varProfile0
 * Signature: (I)[I
 */
jintArray BuDDyBDD_varProfile0__impl
  ( jint b )
{
  jintArray result;
  int size;
  int* arr;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_varnum()\n");
#endif
  size = bdd_varnum();
#if defined(TRACE_BUDDYLIB)
  printf("bdd_varprofile(%d)\n", b);
#endif
  arr = bdd_varprofile(b);
  if (check_error()) return NULL;
  if (arr == NULL) return NULL;
  result = (*thread_env)->NewIntArray(thread_env, size);
  if (result == NULL) return NULL;
  (*thread_env)->SetIntArrayRegion(thread_env, result, 0, size, (jint*) arr);
  free(arr);
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    addRef
 * Signature: (I)V
 */
void BuDDyBDD_addRef__impl
  ( jint b )
{
#if defined(TRACE_BUDDYLIB)
  printf("bdd_addref(%d)\n", b);
#endif
  bdd_addref(b);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    delRef
 * Signature: (I)V
 */
void BuDDyBDD_delRef__impl
  ( jint b )
{
  if (b != INVALID_BDD) {
#if defined(TRACE_BUDDYLIB)
    printf("bdd_delref(%d)\n", b);
#endif
    bdd_delref(b);
    check_error();
  }
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    veccompose0
 * Signature: (IJ)I
 */
jint BuDDyBDD_veccompose0__impl
  ( jint b, jlong pair )
{
  int result;
  bddPair* p;
  p = (bddPair*) (intptr_cast_type) pair;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_veccompose(%d, %p)\n", b, p);
#endif
  result = bdd_veccompose(b, p);
  check_error();
  return result;
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDD
 * Method:    replace0
 * Signature: (IJ)I
 */
jint BuDDyBDD_replace0__impl
  ( jint b, jlong pair )
{
  int result;
  bddPair* p;
  p = (bddPair*) (intptr_cast_type) pair;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_replace(%d, %p)\n", b, p);
#endif
  result = bdd_replace(b, p);
  check_error();
  return result;
}

/* class net_sf_javabdd_BuDDyFactory_BuDDyBDDPairing */

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDDPairing
 * Method:    set0
 * Signature: (JII)V
 */
void BuDDyBDDPairing_set0__impl
  ( jlong pair, jint i, jint j )
{
  bddPair* p;
  p = (bddPair*) (intptr_cast_type) pair;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_setpair(%p, %d, %d)\n", p, i, j);
#endif
  bdd_setpair(p, i, j);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDDPairing
 * Method:    set1
 * Signature: (J[I[I)V
 */
void BuDDyBDDPairing_set1__impl
  ( jlong pair, jintArray arr1, jintArray arr2 )
{
  jint size1, size2;
  jint *a1;
  jint *a2;
  bddPair* p;
  p = (bddPair*) (intptr_cast_type) pair;
  size1 = get_length_jintArray(arr1);
  size2 = get_length_jintArray(arr2);
  if (size1 != size2) {
    throw_new_IllegalArgumentException("array sizes do not match");
    return;
  }
  a1 = get_critical_access_jintArray(arr1);
  if (a1 != NULL) {
    a2 = get_critical_access_jintArray(arr2);
    if (a2 != NULL) {
#if defined(TRACE_BUDDYLIB)
      printf("bdd_setpairs(%p, %p, %p, %d)\n", p, a1, a2, size1);
#endif
      bdd_setpairs(p, (int*)a1, (int*)a2, size1);
      release_critical_access_jintArray(arr2, a2);
    }
    release_critical_access_jintArray(arr1, a1);
  }
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDDPairing
 * Method:    set2
 * Signature: (JII)V
 */
void BuDDyBDDPairing_set2__impl
  ( jlong pair, jint b, jint c )
{
  bddPair* p;
  p = (bddPair*) (intptr_cast_type) pair;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_setbddpair(%p, %d, %d)\n", p, b, c);
#endif
  bdd_setbddpair(p, b, c);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDDPairing
 * Method:    set3
 * Signature: (J[I[I)V
 */
void BuDDyBDDPairing_set3__impl
  ( jlong pair, jintArray arr1, jintArray arr2 )
{
  jint size1, size2;
  bdd *a1;
  bdd *a2;
  bddPair* p;
  p = (bddPair*) (intptr_cast_type) pair;
  size1 = get_length_jintArray(arr1);
  size2 = get_length_jintArray(arr2);
  if (size1 != size2) {
    throw_new_IllegalArgumentException("array sizes do not match");
    return;
  }
  a1 = get_critical_access_jintArray(arr1);
  if (a1 != NULL) {
    a2 = get_critical_access_jintArray(arr2);
    if (a2 != NULL) {
#if defined(TRACE_BUDDYLIB)
      printf("bdd_setbddpairs(%p, %p, %p, %d)\n", p, a1, a2, size1);
#endif
      bdd_setbddpairs(p, (int*)a1, (int*)a2, size1);
      release_critical_access_jintArray(arr2, a2);
    }
    release_critical_access_jintArray(arr1, a1);
  }
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDDPairing
 * Method:    reset0
 * Signature: (J)V
 */
void BuDDyBDDPairing_reset0__impl
  ( jlong pair )
{
  bddPair* p;
  p = (bddPair*) (intptr_cast_type) pair;
#if defined(TRACE_BUDDYLIB)
  printf("bdd_resetpair(%p)\n", p);
#endif
  bdd_resetpair(p);
  check_error();
}

/*
 * Class:     net_sf_javabdd_BuDDyFactory_BuDDyBDDPairing
 * Method:    free0
 * Signature: (J)V
 */
void BuDDyBDDPairing_free0__impl
  ( jlong pair )
{
  bddPair* p;
  p = (bddPair*) (intptr_cast_type) pair;
  if (p) {
#if defined(TRACE_BUDDYLIB)
    printf("bdd_freepair(%p)\n", p);
#endif
    bdd_freepair(p);
  }
}

