//==========================================================================
// digitrec.cpp
//==========================================================================
// @brief: A k-nearest-neighbor kernel for digit recognition 

// As for sw_emu, it only copy digitrec.cpp to a separate directory and only search in it...
// That why I'm using the absolute path...
// Please change these two absolute paths to pass the sw_emu
#include "/home/centos/src/project_data/aws_ap6/AWS_Heterocl/solution/src/host/typedefs.h"
#include "/home/centos/src/project_data/aws_ap6/AWS_Heterocl/solution/src/kernel/functions.cpp"
#include "/home/centos/src/project_data/aws_ap6/AWS_Heterocl/solution/src/kernel/default_function.cpp"

extern "C" 
{
  //----------------------------------------------------------
  // Digitrec
  //----------------------------------------------------------
  // The is the top function of the kernel, reads in the training 
  // data and testing data, return an array of the recognized labels
  //
  // @param[in] : input - the testing instance
  // @return : the recognized digit (0~9)
  void DigitRec( digit* training_data, digit* testing_data, bit6_t* results) 
  {  
    // connection settings:
    // students DO NOT need to understand this
    #pragma HLS INTERFACE m_axi port=training_data offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axi port=testing_data offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axi port=results offset=slave bundle=gmem
    #pragma HLS INTERFACE s_axilite port=training_data bundle=control
    #pragma HLS INTERFACE s_axilite port=testing_data bundle=control
    #pragma HLS INTERFACE s_axilite port=results bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control
    
    // This array stores K minimum distances per training set
    bit6_t knn_set[10 * K_CONST];
    bit6_t knn_mat[10][3];
    digit training_instance[10][NUM_TRAINING];

    // for training data
    for ( int i = 0;i < 10; i++ ) {
      for ( int j = 0;j < NUM_TRAINING; j++ ) {
        digit training_instance[i][j] = training_data[i * NUM_TRAINING + j];
      }
    }

    for ( int k = 0; k < NUM_TEST; k++ ) {
      // for test data
      digit testing_instance = testing_data[k];

      // for knn_mat
      for ( int i = 0;i < 10; i++ ) {
        for ( int j = 0;j < K_CONST; j++ ) {
          knn_mat[i][j] = 0;
        }
      }
      default_function(testing_instance, training_instance, knn_mat);
      for ( int i = 0;i < 10; i++ ) {
        for (int j = 0;j < K_CONST; j++ ) {
          knn_set[i * K_CONST + j] = knn_mat[i][j];
        }
      }
      results[k] = knn_vote(knn_set);
    }

    // for each of the test data
  //   for ( int k = 0 ; k < NUM_TEST; k++ ){
  //     digit testing_instance = testing_data[k];
  //     // Initialize the knn set
  //     // for ( int i = 0; i < 10 * K_CONST; i++ )
  //     //     // Note that the max distance is 49
  //     //     knn_set[i] = 50; 
  //     // for each of the training data
  //     // for ( int i = 0; i < NUM_TRAINING; i++ ){
  //     //   for ( int j = 0; j < 10; j++ ){
  //     //   digit training_instance =  training_data[j* NUM_TRAINING + i];
  //     //   // Update the KNN set
  //     //   // update_knn( testing_instance, training_instance, &knn_set[j * K_CONST] );
  //     //   }
  //     // } 
  //     // collect the results
  //     results[k] = knn_vote(knn_set);
  //   }
  }

}
