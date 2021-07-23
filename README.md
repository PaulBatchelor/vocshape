Vocshape
========

Vocshape is a very simple proof-of-concept musical
instrument for Android that aims to demonstrate
the sculptability of a simple articulatory synthesis
physical model for vocal synthesis.

A brief demo video can be found [on youtube](https://www.youtube.com/watch?v=nyapC6xbS9w).


This was written as a part of the presentation for
my (now rejected) PhD project proposal
[Computer Augmented Techniques for Musical Articulatory Synthesis](https://pbat.ch/wiki/CATFMAS).

Vocshape is written entirely in C using the Native Android
SDK, with graphics beiing done in nanovg via the OpenGLES
backend, and sound being produced via OpenSL.

The DSP used here features code from
[sndkit](https://pbat.ch/sndkit), in particular the
[tract](https://pbat.ch/sndkit/tract) and
[glottis](https://pbat.ch/sndkit/glottis) algorithms
which are the main components used to form a vocal tract
model.

For questions, please email thisispaulbatchelor@gmail.com
