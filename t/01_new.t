use strict;
use warnings;
use Test::More;
use Digest::GOST;

new_ok('Digest::GOST', [], "new");

can_ok('Digest::GOST',
    qw(clone reset add digest hexdigest b64digest)
);

my $d1 = Digest::GOST->new();
$d1->add('foo bar')->reset;
is($d1->hexdigest, Digest::GOST->new()->hexdigest, 'explicit reset');

is(
    eval { $d1->reset->add('a')->digest; $d1->add('a')->hexdigest },
    $d1->reset->add('a')->hexdigest,
    'implicit reset'
);

$d1->add('foobar');
my $d2 = $d1->clone;
is($d1->hexdigest, $d2->hexdigest, 'clone');

done_testing;
