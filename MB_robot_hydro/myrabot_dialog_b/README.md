Pocketsphinx wrapper for ROS from https://github.com/mikeferguson/pocketsphinx/tree/groovy-devel

It is necessary to install the last version of gstreamer, it allows 
to work with Finite State Grammar (FSG) 

The new FSG support appears to require version 0.8 of gstreamer0.10-pocketsphinx and related packages. Ubuntu 12.04 (Precise) uses version 0.5.1 of these pacakges which must be removed and replaced. The 0.8 versions can be found from the Raring repository (http://packages.ubuntu.com/raring/gstreamer0.10-pocketsphinx).

## FRAN: I have not tested this, i just install gstreamer package from ubuntu web (not from repositories)
After removing all 0.5.1 versions of the pocketsphinx packages, install the 0.8 versions of the following packages:

    gstreamer0.10-pocketsphinx
    pocketsphinx-utils
    python-pocketsphinx
    libpocketsphinx1
    pocketsphinx-hmm-en-hub4wsj
    pocketsphinx-lm-en-hub4
    pocketsphinx-lm-wsj
    pocketsphinx-hmm-en-tidigits
    pocketsphinx-hmm-tidigits
    libpocketsphinx-dev
    libsphinxbase-dev
    libsphinxbase1


