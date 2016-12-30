/*!
 *  Auto-generated types for schema definition [source schema: test_schema]
 *
 *  Reference Schema Version : 0001.0001
 *  Generated by : subho
 *  Generated on : 2016-12-30 11:16:08.484
 */


#ifndef TEST_TYPE_H
#define TEST_TYPE_H
#include "types/includes/type_defs.h"
#include "types/includes/__base_type.h"



using namespace REACTFS_NS_COMMON_PREFIX;
using namespace REACTFS_NS_CORE_PREFIX::types;



namespace com {
	namespace wookler {
		namespace test {
			/**
			* Generated code for data record type test_type.
			*
			* Note:: Should not be modified as the changes will be lost when the code is re-generated.
			*/
			class test_type  : public com::wookler::reactfs::core::types::__base_type
			{
			private:
				unordered_map<double, char *> * testMapString = nullptr;

				char * testString = nullptr;

				double * testDouble = nullptr;

				float * testFloat = nullptr;

				vector<char *> * testListString = nullptr;



				/**
				* Value testMapString setter from the the serialized data map.
				*
				* @param __data - Serialized data map pointer.
				*/
				void set_value_testMapString ( __struct_datatype__ *__data )
				{
					if ( NOT_NULL ( __data ) ) {
						__struct_datatype__::const_iterator iter = __data->find ( "testMapString" );
						if ( iter != __data->end() ) {
							void *__ptr = iter->second;
							if ( NOT_NULL ( __ptr ) ) {
								// Set the value of testMapString
								unordered_map<double, char *> * __var = static_cast<unordered_map<double, char *> *> ( __ptr );
								CHECK_CAST ( __var, TYPE_NAME ( void * ), TYPE_NAME ( unordered_map ) );
								this->testMapString = __var;

							} else {
								this->testMapString = nullptr;
							}
						} else {
							this->testMapString = nullptr;
						}
					} else {
						this->testMapString = nullptr;
					}
				}

				/**
				 * Method extracts and set the value of testMapString from
				 * the serialized data map.
				 *
				 * @param __data - Serialized data map pointer.
				 */
				void set_map_testMapString ( __struct_datatype__ *__data )
				{
					CHECK_NOT_NULL ( __data );
					if ( NOT_NULL ( this->testMapString ) ) {
						// Add the value of testMapString to the data map.
						__data->insert ( {"testMapString", this->testMapString} );

					}
				}

				/**
				 * Value testString setter from the the serialized data map.
				 *
				 * @param __data - Serialized data map pointer.
				 */
				void set_value_testString ( __struct_datatype__ *__data )
				{
					if ( NOT_NULL ( __data ) ) {
						__struct_datatype__::const_iterator iter = __data->find ( "testString" );
						if ( iter != __data->end() ) {
							void *__ptr = iter->second;
							if ( NOT_NULL ( __ptr ) ) {
								// Set the value of testString
								char * __var = static_cast<char *> ( __ptr );
								CHECK_CAST ( __var, TYPE_NAME ( void * ), TYPE_NAME ( char ) );
								this->testString = __var;

							} else {
								this->testString = nullptr;
							}
						} else {
							this->testString = nullptr;
						}
					} else {
						this->testString = nullptr;
					}
				}

				/**
				 * Method extracts and set the value of testString from
				 * the serialized data map.
				 *
				 * @param __data - Serialized data map pointer.
				 */
				void set_map_testString ( __struct_datatype__ *__data )
				{
					CHECK_NOT_NULL ( __data );
					if ( NOT_NULL ( this->testString ) ) {
						// Add the value of testString to the data map.
						__data->insert ( {"testString", this->testString} );

					}
				}

				/**
				 * Value testDouble setter from the the serialized data map.
				 *
				 * @param __data - Serialized data map pointer.
				 */
				void set_value_testDouble ( __struct_datatype__ *__data )
				{
					if ( NOT_NULL ( __data ) ) {
						__struct_datatype__::const_iterator iter = __data->find ( "testDouble" );
						if ( iter != __data->end() ) {
							void *__ptr = iter->second;
							if ( NOT_NULL ( __ptr ) ) {
								// Set the value of testDouble
								double * __var = static_cast<double *> ( __ptr );
								CHECK_CAST ( __var, TYPE_NAME ( void * ), TYPE_NAME ( double ) );
								this->testDouble = __var;

							} else {
								this->testDouble = nullptr;
							}
						} else {
							this->testDouble = nullptr;
						}
					} else {
						this->testDouble = nullptr;
					}
				}

				/**
				 * Method extracts and set the value of testDouble from
				 * the serialized data map.
				 *
				 * @param __data - Serialized data map pointer.
				 */
				void set_map_testDouble ( __struct_datatype__ *__data )
				{
					CHECK_NOT_NULL ( __data );
					if ( NOT_NULL ( this->testDouble ) ) {
						// Add the value of testDouble to the data map.
						__data->insert ( {"testDouble", this->testDouble} );

					}
				}

				/**
				 * Value testFloat setter from the the serialized data map.
				 *
				 * @param __data - Serialized data map pointer.
				 */
				void set_value_testFloat ( __struct_datatype__ *__data )
				{
					if ( NOT_NULL ( __data ) ) {
						__struct_datatype__::const_iterator iter = __data->find ( "testFloat" );
						if ( iter != __data->end() ) {
							void *__ptr = iter->second;
							if ( NOT_NULL ( __ptr ) ) {
								// Set the value of testFloat
								float * __var = static_cast<float *> ( __ptr );
								CHECK_CAST ( __var, TYPE_NAME ( void * ), TYPE_NAME ( float ) );
								this->testFloat = __var;

							} else {
								this->testFloat = nullptr;
							}
						} else {
							this->testFloat = nullptr;
						}
					} else {
						this->testFloat = nullptr;
					}
				}

				/**
				 * Method extracts and set the value of testFloat from
				 * the serialized data map.
				 *
				 * @param __data - Serialized data map pointer.
				 */
				void set_map_testFloat ( __struct_datatype__ *__data )
				{
					CHECK_NOT_NULL ( __data );
					if ( NOT_NULL ( this->testFloat ) ) {
						// Add the value of testFloat to the data map.
						__data->insert ( {"testFloat", this->testFloat} );

					}
				}

				/**
				 * Value testListString setter from the the serialized data map.
				 *
				 * @param __data - Serialized data map pointer.
				 */
				void set_value_testListString ( __struct_datatype__ *__data )
				{
					if ( NOT_NULL ( __data ) ) {
						__struct_datatype__::const_iterator iter = __data->find ( "testListString" );
						if ( iter != __data->end() ) {
							void *__ptr = iter->second;
							if ( NOT_NULL ( __ptr ) ) {
								// Set the value of testListString
								vector<char *> * __var = static_cast<vector<char *> *> ( __ptr );
								CHECK_CAST ( __var, TYPE_NAME ( void * ), TYPE_NAME ( vector ) );
								this->testListString = __var;

							} else {
								this->testListString = nullptr;
							}
						} else {
							this->testListString = nullptr;
						}
					} else {
						this->testListString = nullptr;
					}
				}

				/**
				 * Method extracts and set the value of testListString from
				 * the serialized data map.
				 *
				 * @param __data - Serialized data map pointer.
				 */
				void set_map_testListString ( __struct_datatype__ *__data )
				{
					CHECK_NOT_NULL ( __data );
					if ( NOT_NULL ( this->testListString ) ) {
						// Add the value of testListString to the data map.
						__data->insert ( {"testListString", this->testListString} );

					}
				}


			public:
				/**
				* Get the pointer to the property testMapString of type unordered_map<double, char *> *.
				* Returns a const pointer.
				*
				* @return unordered_map<double, char *> *
				*/
				const unordered_map<double, char *> * get_testMapString ( void ) const
				{
					return this->testMapString;
				}

				/**
				 * Set the pointer to testMapString.
				 *
				 * @param testMapString - Pointer of type unordered_map<double, char *> *.
				 */
				void set_testMapString ( unordered_map<double, char *> * testMapString )
				{
					this->testMapString = testMapString;
				}

				/**
				 * Add a map record of key/value type double/char *.
				 *
				 * @param m_key - Map key of type double
				 * @param m_value - String value of the map record value.
				 */
				void add_to_testMapString ( double m_key, string &m_value )
				{
					PRECONDITION ( this->__is_allocated == true );
					if ( IS_NULL ( this->testMapString ) ) {
						this->testMapString = new std::unordered_map<double, char *>();
						CHECK_ALLOC ( this->testMapString, TYPE_NAME ( unordered_map ) );
					}
					CHECK_NOT_EMPTY ( m_value );
					uint32_t __size = ( m_value.length() + 1 ) * sizeof ( char );
					char *__var = ( char * ) malloc ( sizeof ( char ) * __size );
					CHECK_ALLOC ( __var, TYPE_NAME ( char * ) );
					memset ( __var, 0, __size );
					memcpy ( __var, m_value.c_str(), ( __size - 1 ) );
					this->testMapString->insert ( {m_key, __var} );
				}

				/**
				 * Set the pointer to testString.
				 *
				 * @param testString - Pointer of type char *.
				 */
				void set_testString ( char * testString )
				{
					this->testString = testString;
				}

				/**
				 * Get the pointer to the property testString of type char *.
				 * Returns a const pointer.
				 *
				 * @return char *
				 */
				const char * get_testString ( void ) const
				{
					return this->testString;
				}

				/**
				 * Set the char buffer value from the specified string.
				 *
				 * Method should only be used when this instance is being
				 * used to create or update a new record instance.
				 *
				 * @param testString - String value to set.
				 */
				void set_testString ( string &testString )
				{
					FREE_PTR ( this->testString );
					if ( !IS_EMPTY ( testString ) ) {
						uint32_t __size = testString.length() + 1;
						this->testString = ( char * ) malloc ( sizeof ( char ) * __size );
						CHECK_ALLOC ( this->testString, TYPE_NAME ( char * ) );
						memset ( this->testString, 0, __size );
						memcpy ( this->testString, testString.c_str(), ( __size - 1 ) );
					}
				}

				/**
				 * Set the pointer to testDouble.
				 *
				 * @param testDouble - Pointer of type double *.
				 */
				void set_testDouble ( double * testDouble )
				{
					this->testDouble = testDouble;
				}

				/**
				 * Get the pointer to the property testDouble of type double *.
				 * Returns a const pointer.
				 *
				 * @return double *
				 */
				const double * get_testDouble ( void ) const
				{
					return this->testDouble;
				}

				/**
				 * Set the value to testDouble.
				 *
				 * Method should only be used when this instance is being
				 * used to create or update a new record instance.
				 *
				 * @Param testDouble - Value reference.
				 */
				void set_testDouble ( double &testDouble )
				{
					PRECONDITION ( this->__is_allocated == true );
					if ( IS_NULL ( this->testDouble ) ) {
						this->testDouble = ( double * ) malloc ( sizeof ( double ) );
						CHECK_ALLOC ( this->testDouble, TYPE_NAME ( double ) );
					}
					* ( this->testDouble ) = testDouble;
				}

				/**
				 * Set the pointer to testFloat.
				 *
				 * @param testFloat - Pointer of type float *.
				 */
				void set_testFloat ( float * testFloat )
				{
					this->testFloat = testFloat;
				}

				/**
				 * Get the pointer to the property testFloat of type float *.
				 * Returns a const pointer.
				 *
				 * @return float *
				 */
				const float * get_testFloat ( void ) const
				{
					return this->testFloat;
				}

				/**
				 * Set the value to testFloat.
				 *
				 * Method should only be used when this instance is being
				 * used to create or update a new record instance.
				 *
				 * @Param testFloat - Value reference.
				 */
				void set_testFloat ( float &testFloat )
				{
					PRECONDITION ( this->__is_allocated == true );
					if ( IS_NULL ( this->testFloat ) ) {
						this->testFloat = ( float * ) malloc ( sizeof ( float ) );
						CHECK_ALLOC ( this->testFloat, TYPE_NAME ( float ) );
					}
					* ( this->testFloat ) = testFloat;
				}

				/**
				 * Get the pointer to the property testListString of type vector<char *> *.
				 * Returns a const pointer.
				 *
				 * @return vector<char *> *
				 */
				const vector<char *> * get_testListString ( void ) const
				{
					return this->testListString;
				}

				/**
				 * Set the pointer to testListString.
				 *
				 * @param testListString - Pointer of type vector<char *> *.
				 */
				void set_testListString ( vector<char *> * testListString )
				{
					this->testListString = testListString;
				}

				/**
				 * Add a string element to the list testListString.
				 * Elements are expected to be pre-assigned data
				 * value.
				 *
				 * Method should only be used when this instance is being
				 * used to create or update a new record instance.
				 *
				 * @param testListString - String value to add to list.
				 */
				void add_to_testListString ( string &testListString )
				{
					PRECONDITION ( this->__is_allocated == true );
					if ( IS_NULL ( this->testListString ) ) {
						this->testListString = new std::vector<char *>();
						CHECK_ALLOC ( this->testListString, TYPE_NAME ( vector ) );
					}
					if ( !IS_EMPTY ( testListString ) ) {
						uint32_t __size = testListString.length() + 1;
						char *__var = ( char * ) malloc ( sizeof ( char ) * __size );
						CHECK_ALLOC ( __var, TYPE_NAME ( char * ) );
						memset ( __var, 0, __size );
						memcpy ( __var, testListString.c_str(), ( __size - 1 ) );
						this->testListString->push_back ( __var );
					}
				}

				/**
				 * Empty constructor when creating an instance of test_type for setting data locally.
				 * Should be used to create a new record instance of type test_type.
				 */
				test_type()
				{
					this->__is_allocated = true;
					this->testMapString = nullptr;
					this->testString = nullptr;
					this->testDouble = nullptr;
					this->testFloat = nullptr;
					this->testListString = nullptr;

				}

				/**
				 * Create a new read-only instance of test_type and populate the data from the passed
				 * serialized data map.
				 *
				 * @param __data - Serialized data map to load the object data from.
				 */
				test_type ( __struct_datatype__ *__data )
				{
					CHECK_NOT_NULL ( __data );
					this->__is_allocated = false;
					this->testMapString = nullptr;
					this->testString = nullptr;
					this->testDouble = nullptr;
					this->testFloat = nullptr;
					this->testListString = nullptr;

					this->deserialize ( __data );
				}

				/**
				 * Copy constructor to create a copy instance of test_type.
				 * Copy instances should be used to update existing records.
				 *
				 * @param source - Source instance of test_type to copy from.
				 */
				test_type ( const test_type &source )
				{
					this->__is_allocated = true;
					this->testMapString = nullptr;
					this->testString = nullptr;
					this->testDouble = nullptr;
					this->testFloat = nullptr;
					this->testListString = nullptr;

					// Deserialize the map testMapString from the source value.
					if ( NOT_EMPTY_P ( source.testMapString ) ) {
						std::unordered_map<double, char *> *__map = new std::unordered_map<double, char *>();
						CHECK_ALLOC ( __map, TYPE_NAME ( unordered_map ) );

						std::unordered_map<double, char * >::iterator iter;
						for ( iter = source.testMapString->begin(); iter != source.testMapString->end(); iter++ ) {
							CHECK_NOT_NULL ( iter->second );
							char * __tv = ( char * ) malloc ( sizeof ( char ) );
							CHECK_ALLOC ( __tv, TYPE_NAME ( char ) );
							*__tv = * ( iter->second );
							__map->insert ( {iter->first, __tv} );
						}
						this->testMapString = __map;
					}

// Set testString from the source value.
					if ( NOT_NULL ( source.testString ) ) {
						string __testString = string ( source.testString );
						this->set_testString ( __testString );
					}

// Set testDouble from the source value.
					if ( NOT_NULL ( source.testDouble ) ) {
						this->set_testDouble ( * ( source.testDouble ) );
					}

// Set testFloat from the source value.
					if ( NOT_NULL ( source.testFloat ) ) {
						this->set_testFloat ( * ( source.testFloat ) );
					}

// Deserialize the list testListString from the source value.
					if ( NOT_EMPTY_P ( source.testListString ) ) {
						std::vector<char *> *__list = new std::vector<char *>();
						CHECK_ALLOC ( __list, TYPE_NAME ( vector ) );
						for ( char * v : * ( source.testListString ) ) {
							CHECK_NOT_NULL ( v );
							char * __tv = ( char * ) malloc ( sizeof ( char ) );
							CHECK_ALLOC ( __tv, TYPE_NAME ( char ) );
							*__tv = *v;
							__list->push_back ( __tv );
						}
						this->testListString = __list;
					}


				}

				/**
				 * Copy constructor to create a copy instance of test_type.
				 * Copy instances should be used to update existing records.
				 *
				 * @param source - Source instance of test_type to copy from.
				 */
				test_type ( const test_type *source )
				{
					this->__is_allocated = true;
					this->testMapString = nullptr;
					this->testString = nullptr;
					this->testDouble = nullptr;
					this->testFloat = nullptr;
					this->testListString = nullptr;

					// Deserialize the map testMapString from the source value.
					if ( NOT_EMPTY_P ( source->testMapString ) ) {
						std::unordered_map<double, char *> *__map = new std::unordered_map<double, char *>();
						CHECK_ALLOC ( __map, TYPE_NAME ( unordered_map ) );

						std::unordered_map<double, char * >::iterator iter;
						for ( iter = source->testMapString->begin(); iter != source->testMapString->end(); iter++ ) {
							CHECK_NOT_NULL ( iter->second );
							char * __tv = ( char * ) malloc ( sizeof ( char ) );
							CHECK_ALLOC ( __tv, TYPE_NAME ( char ) );
							*__tv = * ( iter->second );
							__map->insert ( {iter->first, __tv} );
						}
						this->testMapString = __map;
					}

// Set testString from the source value.
					if ( NOT_NULL ( source->testString ) ) {
						string __testString = string ( source->testString );
						this->set_testString ( __testString );
					}

// Set testDouble from the source value.
					if ( NOT_NULL ( source->testDouble ) ) {
						this->set_testDouble ( * ( source->testDouble ) );
					}

// Set testFloat from the source value.
					if ( NOT_NULL ( source->testFloat ) ) {
						this->set_testFloat ( * ( source->testFloat ) );
					}

// Deserialize the list testListString from the source value.
					if ( NOT_EMPTY_P ( source->testListString ) ) {
						std::vector<char *> *__list = new std::vector<char *>();
						CHECK_ALLOC ( __list, TYPE_NAME ( vector ) );
						for ( char * v : * ( source->testListString ) ) {
							CHECK_NOT_NULL ( v );
							char * __tv = ( char * ) malloc ( sizeof ( char ) );
							CHECK_ALLOC ( __tv, TYPE_NAME ( char ) );
							*__tv = *v;
							__list->push_back ( __tv );
						}
						this->testListString = __list;
					}


				}

				/**
				 * Descructor for test_type
				 */
				~test_type()
				{
					if ( this->__is_allocated ) {
						FREE_NATIVE_MAP ( this->testMapString );
					} else {
						this->testMapString->clear();
						CHECK_AND_FREE ( this->testMapString );
					}

					if ( this->__is_allocated ) {
						FREE_PTR ( this->testString );
					}

					if ( this->__is_allocated ) {
						FREE_PTR ( this->testDouble );
					}

					if ( this->__is_allocated ) {
						FREE_PTR ( this->testFloat );
					}

					if ( this->__is_allocated ) {
						FREE_NATIVE_LIST ( this->testListString );
					} else {
						this->testListString->clear();
						CHECK_AND_FREE ( this->testListString );
					}


					__base_type::free_data_ptr();
				}

				/**
				 * Free an instance of the serialized data map.
				 *
				 * @param __data - Pointer to the instance of a serialized data map.
				 * @param allocated - Are the value pointers locally allocated and should be freed?
				 */
				void free_data_ptr ( __struct_datatype__ *__data, bool allocated ) override
				{
					if ( NOT_NULL ( __data ) ) {
						__struct_datatype__::iterator iter;
						// Free the map pointer and the list values, if locally allocated.
						iter = __data->find ( "testMapString" );
						if ( iter != __data->end() ) {
							if ( NOT_NULL ( iter->second ) ) {
								void *__ptr = iter->second;
								std::unordered_map<double, char *> *__m_ptr = static_cast<std::unordered_map<double, char *> *> ( __ptr );
								CHECK_CAST ( __m_ptr, TYPE_NAME ( void ), TYPE_NAME ( unordered_map ) );
								if ( allocated ) {
									FREE_NATIVE_MAP ( __m_ptr );
								} else {
									CHECK_AND_FREE ( __m_ptr );
								}
							}
						}

// Free the testString value if locally allocated
						iter = __data->find ( "testString" );
						if ( iter != __data->end() ) {
							if ( NOT_NULL ( iter->second ) ) {
								void *__ptr = iter->second;
								if ( allocated ) {
									FREE_PTR ( __ptr );
								}
							}
						}

// Free the testDouble value if locally allocated
						iter = __data->find ( "testDouble" );
						if ( iter != __data->end() ) {
							if ( NOT_NULL ( iter->second ) ) {
								void *__ptr = iter->second;
								if ( allocated ) {
									FREE_PTR ( __ptr );
								}
							}
						}

// Free the testFloat value if locally allocated
						iter = __data->find ( "testFloat" );
						if ( iter != __data->end() ) {
							if ( NOT_NULL ( iter->second ) ) {
								void *__ptr = iter->second;
								if ( allocated ) {
									FREE_PTR ( __ptr );
								}
							}
						}

// Free the list pointer and the list values, if locally allocated.
						iter = __data->find ( "testListString" );
						if ( iter != __data->end() ) {
							if ( NOT_NULL ( iter->second ) ) {
								void *__ptr = iter->second;
								std::vector<char *> *__v_ptr = static_cast<std::vector<char *> *> ( __ptr );
								CHECK_CAST ( __v_ptr, TYPE_NAME ( void ), TYPE_NAME ( vector ) );
								if ( allocated ) {
									FREE_NATIVE_LIST ( __v_ptr );
								} else {
									CHECK_AND_FREE ( __v_ptr );
								}
							}
						}


						__data->clear();
						CHECK_AND_FREE ( __data );
					}
				}

				/**
				 * Deserialize this type instance from the passed data map.
				 *
				 * @param __data - Serialized data map instance to load this type from.
				 */
				void deserialize ( __struct_datatype__ *__data ) override
				{
					CHECK_NOT_NULL ( __data );
					// Set the value of testMapString from the serialized data map.
					this->set_value_testMapString ( __data );

					// Set the value of testString from the serialized data map.
					this->set_value_testString ( __data );

					// Set the value of testDouble from the serialized data map.
					this->set_value_testDouble ( __data );

					// Set the value of testFloat from the serialized data map.
					this->set_value_testFloat ( __data );

					// Set the value of testListString from the serialized data map.
					this->set_value_testListString ( __data );


					this->__data = __data;
				}

				/**
				 * Serialize this type instance into a data map instance.
				 *
				 * @return - Serialized data map instance.
				 */
				__struct_datatype__ *serialize() override
				{
					__struct_datatype__ *__data = new __struct_datatype__();
					CHECK_ALLOC ( __data, TYPE_NAME ( __struct_datatype__ ) );
					// Call the method to add the value of testMapString to the serialized data map.
					this->set_map_testMapString ( __data );

// Call the method to add the value of testString to the serialized data map.
					this->set_map_testString ( __data );

// Call the method to add the value of testDouble to the serialized data map.
					this->set_map_testDouble ( __data );

// Call the method to add the value of testFloat to the serialized data map.
					this->set_map_testFloat ( __data );

// Call the method to add the value of testListString to the serialized data map.
					this->set_map_testListString ( __data );


					return __data;
				}


			};

		}

	}

}

#endif // TEST_TYPE_H

