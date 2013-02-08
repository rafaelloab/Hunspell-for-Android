#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <android/log.h>


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT
jint
JNICALL
Java_com_poc_spellchecker_Hunspell_init(JNIEnv* env,jobject thiz, jstring aff, jstring dic);


JNIEXPORT
jstring
JNICALL
Java_com_poc_spellchecker_Hunspell_test(JNIEnv* env, jobject thiz, jstring wordList);

JNIEXPORT
void
JNICALL
Java_com_poc_spellchecker_Hunspell_finalize(JNIEnv* env,jobject thiz);

JNIEXPORT
jint
JNICALL
Java_com_poc_spellchecker_Hunspell_spell(JNIEnv* env,jobject thiz, jstring word);

JNIEXPORT
jobjectArray
Java_com_poc_spellchecker_Hunspell_suggest(JNIEnv* env,jobject thiz, jstring word);




#ifdef __cplusplus
}
#endif

