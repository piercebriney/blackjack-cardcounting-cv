The backgrounds used to generate the card data are stored on Google Drive. They can be found [here](https://drive.google.com/drive/folders/1NGfgvB5JoNERvCv7k1t2m4dwVxH4Ml30) and should be imported to the backgrounds directory in order to generate a good dataset. Right now, there is only a black background in the backgrounds directory (required for test data generation).

Prerequisites:
Python3 installed

Run `./setup.sh` to create a virtual environment and install the required packages. You will need to re-run this script if you update the DATASET_PATH environment variable.

Before any of the generation scripts, modify the environment variables in .env

To generate a dataset, run `./generate_training_dataset.sh`. This will generate both train and val datasets.

To generate test images that vary by distance and angle, run `./generate_test_dataset.sh`

To generate confusion matrices from the test data, run `./generate_confusion_matrices.sh`. A model must be present for this step to work. One of our models can be downloaded from [here](https://drive.google.com/file/d/186lOh_5cNEld2br0ctzd5X4eKlssSJ-I/view?usp=drive_link). Make sure you are logged into a TAMU account to access it.