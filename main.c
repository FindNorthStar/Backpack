#include <stdio.h>

/*
 * a,b,c,d stands for the value
 * x,y,z,t stands for the num for eggs
 * target stands for the target value
 * 1) if a*x+b*y+c*z+d*t < target, use max selected (a,b,c,d) since cannot satisfy the target
 * 2) if a*x+b*y+c*z+d*t > target, use greedy or Dynamic programming to get result close to the target
 *   2.1) return the num of selected (a,b,c,d)
 */

struct Value {
    int a;
    int b;
    int c;
    int d;
};

struct Select {
    int x;
    int y;
    int z;
    int t;
    int current; // 0->x, 1->y, 2->z, 3->t
};

int get_val(struct Value *value, struct Select *select)
{
    return select->x * value->a + select->y * value->b + select->z * value->c + select->t * value->d;
}

int check_target(struct Value *value, struct Select *select, int target)
{
    int sum = get_val(value, select);
    if (sum < target) {
        return -1;
    } else {
        return 0;
    }
}

int get_greedy_result(struct Value *v, struct Select *s, struct Select *r, int target)
{
    int sum = 0;
    while (sum < target) {
        sum = get_val(v, r);
        if (sum >= target) {
            printf("sum satisfied in first phase, sum: %d\n", sum);
            break;
        }
        // pick type a
        if (r->x < s->x) {
            (r->x)++;
            r->current = 0;
            continue;
        }
        // pick type b
        if (r->y < s->y) {
            (r->y)++;
            r->current = 1;
            continue;
        }
        // pick type c
        if (r->z < s->z) {
            (r->z)++;
            r->current = 2;
            continue;
        }
        // pick type b
        if (r->t < s->t) {
            (r->t)++;
            r->current = 3;
            continue;
        }
    }
    return sum;
}

int get_remove_greedy_result(struct Value *v, struct Select *s, struct Select *r, int target, int current)
{
    int sum = 0;
    while (sum < target && r->t < s->t) {
        sum = get_val(v, r);
        if (sum >= target) {
            printf("sum satisfied in second phase, sum: %d\n", sum);
            break;
        }
        // pick type a
        if (r->x < s->x && current < 0) {
            (r->x)++;
            r->current = 0;
            continue;
        }
        // pick type b
        if (r->y < s->y && current < 1) {
            (r->y)++;
            r->current = 1;
            continue;
        }
        // pick type c
        if (r->z < s->z && current < 2) {
            (r->z)++;
            r->current = 2;
            continue;
        }
        // pick type b
        if (r->t < s->t && current < 3) {
            (r->t)++;
            r->current = 3;
            continue;
        }
    }
    return sum;
}

int remove_greedy_result(struct Value *v, struct Select *s, struct Select *r, int old_sum, int target)
{
    int sum = old_sum;
    while ((sum > target) && (r->current < 3)) {
        // try to remove item
        // remove type a
        if (r->current == 0) {
            r->x--;
            sum = get_remove_greedy_result(v, s, r, target, r->current);
            if (sum < target) {
                r->x++;
                printf("Cannot satisfy with remove value %d\n", v->a);
                break;
            }
        }
        // remove type b
        if (r->current == 1) {
            r->y--;
            sum = get_remove_greedy_result(v, s, r, target, r->current);
            if (sum < target) {
                r->y++;
                printf("Cannot satisfy with remove value %d\n", v->b);
                break;
            }
        }
        // remove type c
        if (r->current == 2) {
            r->z--;
            sum = get_remove_greedy_result(v, s, r, target, r->current);
            if (sum < target) {
                r->z++;
                printf("Cannot satisfy with remove value %d\n", v->c);
                break;
            }
        }
        // previous we have chosen type d
    }
    return sum;
}

void helper_greedy(struct Value *v, struct Select *s, int target)
{
    int sum = 0;
    struct Select result = { 0 };

    if (check_target(v, s, target) != 0) {
        sum = get_val(v, s);
        printf("Cannot satisfy, select num a: %d, b: %d, c: %d, d: %d, sum: %d, target: %d\n",
               s->x, s->y, s->z, s->t, sum, target);
        return;
    }

    sum = get_greedy_result(v, s, &result, target); // current = 0 means we can choose all values

    printf("Can satisfy, preliminary select num a: %d, b: %d, c: %d, d: %d, sum: %d, target: %d, current = %d\n",
           result.x, result.y, result.z, result.t, sum, target, result.current);

    sum = remove_greedy_result(v, s, &result, sum, target);

    printf("Can satisfy, select num a: %d, b: %d, c: %d, d: %d, sum: %d, target: %d, current = %d\n",
           result.x, result.y, result.z, result.t, sum, target, result.current);
}

void helper_dynamic(struct Value *v, struct Select *s, int target)
{
    int sum = 0;
    struct Select result = { 0 };

    if (check_target(v, s, target) != 0) {
        sum = get_val(v, s);
        printf("Cannot satisfy, select num a: %d, b: %d, c: %d, d: %d, sum: %d, target: %d\n",
               s->x, s->y, s->z, s->t, sum, target);
        return;
    }

    // TODO
}

void init_input(struct Value *v, struct Select *s,
                int a, int b, int c, int d,
                int x, int y, int z, int t)
{
    v->a = a;
    v->b = b;
    v->c = c;
    v->d = d;
    s->x = x;
    s->y = y;
    s->z = z;
    s->t = t;
}

int main()
{
    struct Value value;
    struct Select select;
//    init_input(&value, &select,
//               13,7,3,2,
//               1,2,3,4);
//    helper_greedy(&value, &select
//                  50);
    init_input(&value, &select,
               13,7,3,1,
               20,30,30,40);
    helper_greedy(&value, &select, 50);
    return 0;
}
