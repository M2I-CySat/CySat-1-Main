# Ground Station Front End

## Setting up a Virtual Environment

### Python Virtual Environments
Python Virtual Environments allow you to compartmentalize Python install binaries and installed libraries.
It is recommended to use some form of virtual environment when working with the Ground Station Front End
in order to ensure dependency consistency, especially on machines with multiple Python installs. 

NOTE: Whichever Python version you use to create the virtual environment will be the version that the virtual environment packages
into the environment.

More detailed documentation below:
https://docs.python.org/3/library/venv.html
https://packaging.python.org/guides/installing-using-pip-and-virtual-environments/

Otherwise, you are welcome to use any environment/package manager you choose (Anaconda, etc.), but be aware
that the following steps will not apply.

Some quick commands to set up a virtual environment for CySat Ground Station (assuming Windows - see links for UNIX systems)

Install virtualenv:

    py -m pip install --user virtualenv     #--user if local install

Creating a virtualenv:

    py -m venv env      # will create directory and subfiles called env/

Starting a virtualenv:

    .\env\Scripts\activate      #.ps1 on Powershell, .bat on cmd.exe

### Dependency Management
For now we are using a requirements.txt file in order to track dependencies
You may use this file to install all listed dependencies into your virtual environment with the following command:

    pip install -r requirements.txt

If you add/update a dependency, run the following command to update the requirements.txt:

    pip freeze > requirements.txt

## Running the front end
Once you've installed all dependencies, run the front end GUI with the following command:

    python GroundStationInterface.py



       