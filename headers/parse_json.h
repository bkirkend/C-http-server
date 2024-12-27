typedef enum _Token {
  JSON_OBJECT_START, // '{'
  JSON_OBJECT_END,   // '}'
  JSON_LIST_START,   // '['
  JSON_LIST_END,     // ']'
  JSON_KEY,
  JSON_STRING,
  JSON_NUMBER,
  JSON_BOOL_TRUE,
  JSON_BOOL_FALSE,
  JSON_NULL,
  JSON_COLON,       // ':'
  JSON_COMMA        // ','
} JSONToken;

typedef enum _ValType{
  STRING,
  BOOL,
  LIST
} ValType;

typedef struct _JsonType{
  char *key;
  int keylen;
  ValType valtype;
  void *val;
  int vallen;
} JsonType;
