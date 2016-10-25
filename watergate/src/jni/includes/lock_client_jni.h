/* 
 * Copyright [2016] [Subhabrata Ghosh] 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at 
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 *       limitations under the License. 
 * 
 */
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_watergate_library_LockControlClient */


#ifndef _Included_com_watergate_library_LockControlClient
#define _Included_com_watergate_library_LockControlClient
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_watergate_library_LockControlClient
 * Method:    create
 * Signature: (Ljava/lang/String;)V
 */

JNIEXPORT void JNICALL Java_com_watergate_library_LockControlClient_create
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_watergate_library_LockControlClient
 * Method:    findLockByName
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */

JNIEXPORT jstring JNICALL Java_com_watergate_library_LockControlClient_findLockByName
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     com_watergate_library_LockControlClient
 * Method:    getQuota
 * Signature: (Ljava/lang/String;)D
 */

JNIEXPORT jdouble JNICALL Java_com_watergate_library_LockControlClient_getQuota
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_watergate_library_LockControlClient
 * Method:    lock
 * Signature: (Ljava/lang/String;ID)I
 */

JNIEXPORT jint JNICALL Java_com_watergate_library_LockControlClient_lock__Ljava_lang_String_2ID
  (JNIEnv *, jobject, jstring, jint, jdouble);

/*
 * Class:     com_watergate_library_LockControlClient
 * Method:    lock
 * Signature: (Ljava/lang/String;IDJ)I
 */

JNIEXPORT jint JNICALL Java_com_watergate_library_LockControlClient_lock__Ljava_lang_String_2IDJ
  (JNIEnv *, jobject, jstring, jint, jdouble, jlong);

/*
 * Class:     com_watergate_library_LockControlClient
 * Method:    register_thread
 * Signature: (Ljava/lang/String;)V
 */

JNIEXPORT void JNICALL Java_com_watergate_library_LockControlClient_register_1thread
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_watergate_library_LockControlClient
 * Method:    release
 * Signature: (Ljava/lang/String;I)Z
 */

JNIEXPORT jboolean JNICALL Java_com_watergate_library_LockControlClient_release
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     com_watergate_library_LockControlClient
 * Method:    getControlState
 * Signature: ()I
 */

JNIEXPORT jint JNICALL Java_com_watergate_library_LockControlClient_getControlState
  (JNIEnv *, jobject);

/*
 * Class:     com_watergate_library_LockControlClient
 * Method:    getControlError
 * Signature: ()Ljava/lang/String;
 */

JNIEXPORT jstring JNICALL Java_com_watergate_library_LockControlClient_getControlError
  (JNIEnv *, jobject);

/*
 * Class:     com_watergate_library_LockControlClient
 * Method:    dispose
 * Signature: ()V
 */

JNIEXPORT void JNICALL Java_com_watergate_library_LockControlClient_dispose
  (JNIEnv *, jobject);

/*
 * Class:     com_watergate_library_LockControlClient
 * Method:    test_assert
 * Signature: ()V
 */

JNIEXPORT void JNICALL Java_com_watergate_library_LockControlClient_test_1assert
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
/* Header for class com_watergate_library_LockControlClient_EResourceType */


#ifndef _Included_com_watergate_library_LockControlClient_EResourceType
#define _Included_com_watergate_library_LockControlClient_EResourceType
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif
#endif
/* Header for class com_watergate_library_LockControlClient_ELockResult */


#ifndef _Included_com_watergate_library_LockControlClient_ELockResult
#define _Included_com_watergate_library_LockControlClient_ELockResult
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif
#endif
