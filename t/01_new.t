use strict;
use warnings;
use Test::More tests => 4;
use Digest::GOST;

new_ok('Digest::GOST', [], "new");

can_ok('Digest::GOST',
    qw(clone reset add digest hexdigest b64digest)
);

my $d1 = Digest::GOST->new();
$d1->add('foo bar')->reset;
is($d1->hexdigest, Digest::GOST->new()->hexdigest, 'reset');

$d1->add('foobar');
my $d2 = $d1->clone;
is($d1->hexdigest, $d2->hexdigest, "clone");
